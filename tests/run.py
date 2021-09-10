#!/usr/bin/env python3

from multiprocessing.pool import ThreadPool
import multiprocessing
import os.path
import argparse
import yaml
from smack_sgx import run_smack, tally_result, Verifier
import logging
import sys

conflicts = 0

def get_bcfile_path(bcfile):
    prefix = os.path.dirname(os.path.abspath(args.config_file))
    return os.path.join(prefix, bcfile)


def process_result(expected_result):
    def do(result):
        global conflicts
        tally_result(result)
        if (expected_result != 'unknown' and
                result['status'] != expected_result):
            logging.info('{0}: expected result mismatch ({1} vs. {2})!'.format(
                    result['entrypoint'], expected_result, result['status']))
            conflicts += 1
    return do


def run_all(benchmarks, args):
    p = ThreadPool(processes=args.n_threads)
    results = []
    verifiers = [Verifier.BOOGIE] + ([] if args.ci else [Verifier.CORRAL])
    try:
        for benchmark in benchmarks:
            bcfile = get_bcfile_path(benchmark['bc_file'])
            entrypoint = benchmark['entry_point']
            expected_result = benchmark['expected_result']
            for verifier in verifiers:
                r = p.apply_async(
                    run_smack,
                    args=(
                        verifier,
                        bcfile,
                        entrypoint),
                    callback=process_result(expected_result)
                )
                results.append(r)
        for r in results:
            r.wait()

    except KeyboardInterrupt:
        p.terminate()
        p.join()
    else:
        p.close()
        p.join()


def get_benchmarks(config_file):
    with open(config_file, 'r') as f:
        benchmarks = yaml.safe_load(f)['samples']
    return benchmarks


def argument():
    num_cpus = multiprocessing.cpu_count()
    parser = argparse.ArgumentParser()

    parser.add_argument(
        'config_file',
        metavar='FILE',
        type=str,
        help='input configuration file in YAML format')

    parser.add_argument(
        '-n',
        metavar='NUM',
        action='store',
        dest='n_threads',
        default=num_cpus,
        type=int,
        help='number of threads run in parallel [default: %(default)s]')

    parser.add_argument(
      '-ci',
      action='store_true',
      default=True,
      help='enable CI mode')

    return parser.parse_args()


if __name__ == '__main__':
    logging.basicConfig(format='', level=logging.DEBUG)
    args = argument()
    logging.debug("Creating Pool with '%d' workers" % args.n_threads)
    benchmarks = get_benchmarks(args.config_file)
    run_all(benchmarks, args)
    if conflicts > 0:
        sys.exit(1)
