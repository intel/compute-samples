#!/bin/bash

#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading Level Zero"
wget https://github.com/oneapi-src/level-zero/archive/v0.91.zip -O level-zero-0.91.zip

echo "Extracting Level Zero"
unzip -q level-zero-0.91.zip

echo "Installing Level Zero"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/third_party"
LEVEL_ZERO_PATH="$INSTALL_PATH/level_zero"

pushd .
cd level-zero-0.91
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX="$LEVEL_ZERO_PATH"
cmake --build . --target install
popd

echo "Cleaning Level Zero"
rm -rf level-zero-0.91
rm level-zero-0.91.zip
