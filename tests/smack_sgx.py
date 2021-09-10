#!/usr/bin/env python3

from enum import Enum
import time
import subprocess
import os.path
import re
import argparse
import logging


class Verifier(Enum):
    BOOGIE = 1
    CORRAL = 2

    def get_opts(self):
        opts = {Verifier.BOOGIE: ['--verifier=boogie'],
                Verifier.CORRAL: ['--verifier-options=/trackAllVars']}
        return opts[self]

    def __str__(self):
        return self.name.lower()

    def __repr__(self):
        return str(self)

    @staticmethod
    def argparse(s):
        try:
            return Verifier[s.upper()]
        except KeyError:
            raise RuntimeError('Failed to parse verifier option')


def run_smack(verifier, bcfile, entrypoint):
    def get_clue_code_entrypoint():
        return 'sgx_' + entrypoint

    def get_result(output):
        if re.search(r'SMACK timed out', output):
            return 'timeout'
        elif re.search(r'SMACK found no errors', output):
            return 'verified'
        elif re.search(r'SMACK found an error', output):
            return 'error'
        else:
            return 'unknown'

    def write_log(output):
        prefix = os.path.dirname(bcfile)
        logfile = os.path.join(
                               prefix,
                               entrypoint + '-' + str(verifier) + '.log')
        with open(logfile, 'w') as f:
            f.write(output)
        return logfile

    def add_error_info(result, output):
        if result['status'] == 'error' and verifier is Verifier.CORRAL:
            filename = r'[\w#$~%.\/-]+'
            errorP = re.compile(
                ('('
                 + filename
                 + r')\((\d+),(\d+)\): Trace: Thread=\d+\s+\(ASSERTION FAILS'))
            errorMatch = errorP.search(output)
            assert errorMatch, 'Failed to obtain assertion failure info!'
            result['error_info'] = '{0}({1},{2})'.format(
                errorMatch.group(1),
                errorMatch.group(2),
                errorMatch.group(3))

    cmd = ['smack',
           bcfile,
           '--entry-point',
           get_clue_code_entrypoint(),
           '--sgx',
           '--check',
           'valid-deref',
           'valid-free',
           '--unroll=17',
           '--time-limit=1200000000',
           '-v',
           '--only-check-enclave-memory',
           '--havoc-outside-memory',
           '--enable-null-check',
           '--enable-failing-malloc']
    cmd += verifier.get_opts()
    p = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
        )
    t0 = time.time()

    out, err = p.communicate()
    elapsed = '%.2fs' % round(time.time() - t0, 2)
    result = {}
    result['status'] = get_result(out)
    result['time'] = elapsed
    result['verifier'] = str(verifier)
    result['entrypoint'] = entrypoint
    result['cmd'] = ' '.join(cmd)
    result['logfile'] = write_log(out)
    result['dir'] = os.path.split(os.path.dirname(bcfile))[1]
    add_error_info(result, out)
    return result


def tally_result(result):
    def get_status():
        if result['verifier'] == 'boogie' or result['status'] != 'error':
            return result['status']
        else:
            return '{0} at {1}'.format(result['status'], result['error_info'])
    logging.debug('{0}/{1}: verifier {2}, status {3}, time {4}'.format(
        result['dir'],
        result['entrypoint'],
        result['verifier'],
        get_status(),
        result['time']))


def argument():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        'input_file',
        metavar='FILE',
        type=str,
        help='input bitcode file')

    parser.add_argument(
        '--verifier',
        choices=list(Verifier),
        default=Verifier.CORRAL,
        type=Verifier.argparse,
        help='back-end verification engine')

    parser.add_argument(
        '--entry-point',
        metavar='PROC',
        type=str,
        help='specify top-level procedure')

    return parser.parse_args()


if __name__ == '__main__':
    logging.basicConfig(format='', level=logging.DEBUG)
    args = argument()
    tally_result(run_smack(args.verifier, args.input_file, args.entry_point))
