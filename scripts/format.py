#
# Copyright(c) 2018 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import argparse
import glob
import logging
import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from itertools import repeat


def main():
    logging.basicConfig(level=logging.INFO, format='%(message)s')

    logging.info('Processing a command line')
    args = process_command_line()

    logging.info('Finding source files')
    files = find_source_files(args.root_directory)

    logging.info('Running clang-format')
    with ThreadPoolExecutor() as executor:
        executor.map(run_clang_format_star,
                     zip(files, repeat(args.clang_format)))

    logging.info('Formatting is done')


def find_source_files(root_path):
    types = ('.cpp', '.hpp', '.cl')
    files = []
    for t in types:
        pattern = '{}/**/*{}'.format(root_path, t)
        for f in glob.iglob(pattern, recursive=True):
            files.append(os.path.normpath(f))
    return files


def run_clang_format_star(args):
    run_clang_format(*args)


def run_clang_format(f, command='clang-format'):
    cmd = '{} -i {}'.format(command, f)
    subprocess.run(cmd, shell=True, check=True)


def process_command_line():
    parser = argparse.ArgumentParser()
    parser.add_argument('--root-directory', default='../compute_samples',
                        help='Root directory of compute-samples source')
    parser.add_argument('--clang-format', default='clang-format',
                        help='clang-format command')
    return parser.parse_args()


if __name__ == '__main__':
    main()
