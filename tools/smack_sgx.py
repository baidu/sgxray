#!/usr/bin/env python3

import argparse
from enum import Enum
import subprocess
import sys
import os


class Verifier(Enum):
    BOOGIE = 1
    CORRAL = 2

    def get_opts(self):
        opts = {Verifier.BOOGIE: ['--verifier=boogie'],
                Verifier.CORRAL:
                    ["--verifier-options", "/trackAllVars /staticInlining"]}
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


def arguments():
    """Parse command-line arguments"""

    parser = argparse.ArgumentParser()

    parser.add_argument(
        'input_file',
        metavar='BC_FILE',
        type=str,
        help='source file to be verified')

    sdk_type = parser.add_mutually_exclusive_group(required=True)

    sdk_type.add_argument(
        '--intel',
        default=False,
        action='store_true',
        help='use Intel SGX SDK')

    sdk_type.add_argument(
        '--oe',
        default=False,
        action='store_true',
        help='use openenclave SDK')

    parser.add_argument(
        '--ecall-name',
        metavar='PROC',
        type=str,
        required=True,
        help='the name of the ecall to be verified')

    parser.add_argument(
        '--verifier',
        choices=list(Verifier),
        default=Verifier.BOOGIE,
        type=Verifier.argparse,
        help='back-end verification engine, [default: %(default)s]')

    parser.add_argument(
        '--smack-options',
        metavar='OPTIONS',
        default='',
        type=str,
        help='SMACK options (e.g., --smack-options="--unroll 2")')

    parser.add_argument(
        '-d', '--debug',
        default=False,
        action='store_true',
        help='enable debug mode (e.g., show SMACK invocation)')

    return parser.parse_args()


def run_smack(args):
    def get_entrypoint():
        if args.intel:
            return 'sgx_' + args.ecall_name
        else:
            return 'main'

    err_file = args.input_file + '-' + args.ecall_name + '.log'

    cmd = ['smack',
           args.input_file,
           '--entry-point',
           get_entrypoint(),
           '--sgx',
           '--check',
           'valid-deref',
           'valid-free',
           '-w=%s' % err_file,
           '--warn=silent',
           '--integer-encoding=wrapped-integer',
           '--only-check-enclave-memory',
           '--havoc-outside-memory'] + args.smack_options.split()

    cmd += args.verifier.get_opts()

    if args.oe:
        cmd += ['--ecall-name', 'ecall_' + args.ecall_name]
        sgx_lib_path = os.path.join(
                                    os.path.dirname(
                                        os.path.realpath(__file__)),
                                    'lib',
                                    'sgx-libs')
        cmd += ['--sgx-sdk-libs',
                os.path.join(sgx_lib_path, 'oe-libs', 'liboecore.a.bc'),
                os.path.join(sgx_lib_path, 'oe-libs', 'dummy_main.bc')]

    if args.debug:
        print(' '.join(map(lambda x: '"' + x + '"', cmd)))

    p = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
        )

    out, err = p.communicate()
    print(out+err, end='', flush=True)


def main():
    try:
        global args
        args = arguments()
        run_smack(args)

    except KeyboardInterrupt:
        sys.exit("SGX-Ray aborted by keyboard interrupt.")
