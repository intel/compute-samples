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
import shutil


def main():
    logging.basicConfig(level=logging.INFO, format='%(message)s')

    logging.info('Processing a command line')
    args = process_command_line()

    logging.info('Preparing paths')
    applications_directory = os.path.normpath(os.path.join(args.root_directory, 'compute_samples/applications'))
    target_directory = os.path.join(applications_directory, args.name)
    template_directory = os.path.normpath('./template')

    logging.info('Copying a template')
    copy_template(template_directory, target_directory)

    logging.info('Adding a CMake subdirectory')
    add_cmake_subdirectory(applications_directory, args.name)

    logging.info('Renaming files')
    rename_files(target_directory, args.name)

    logging.info('Renaming tokens')
    rename_tokens(target_directory, args.name)

    logging.info('Application is successfully generated')


def process_command_line():
    parser = argparse.ArgumentParser()
    parser.add_argument('name', help='Target name in the lower_snake_case format')
    parser.add_argument('--root-directory', default='..', help='Root directory of compute-samples repository')
    return parser.parse_args()


def copy_template(src, dst):
    shutil.copytree(src, dst)


def add_cmake_subdirectory(applications_directory, target_name):
    cmake_file = os.path.join(applications_directory, 'CMakeLists.txt')
    with open(cmake_file, 'a', newline='') as f:
        f.write('add_subdirectory({})\n'.format(target_name))


def rename_files(target_directory, target_name):
    for filename in glob.iglob(target_directory + '/**/*', recursive=True):
        if 'template' in filename:
            os.rename(filename, filename.replace('template', target_name))


def rename_tokens(target_directory, target_name):
    for filename in glob.iglob(target_directory + '/**/*.*', recursive=True):
        with open(filename, newline='') as f:
            data = f.read()

        data = data.replace('template', target_name)
        data = data.replace('Template', upper_camel_case(target_name))
        data = data.replace('TEMPLATE', upper_snake_case(target_name))

        with open(filename, 'w', newline='') as f:
            f.write(data)


def upper_camel_case(snake_case):
    return snake_case.title().replace('_', '')


def upper_snake_case(snake_case):
    return snake_case.upper()


if __name__ == '__main__':
    main()
