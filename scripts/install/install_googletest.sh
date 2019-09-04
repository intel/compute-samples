#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading googletest"
wget https://github.com/google/googletest/archive/release-1.8.1.zip -O googletest-release-1.8.1.zip

echo "Extracting googletest"
unzip -q googletest-release-1.8.1.zip

echo "Installing googletest"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/third_party"
GOOGLETEST_PATH="$INSTALL_PATH/googletest"

mkdir -p $GOOGLETEST_PATH
cp -R googletest-release-1.8.1/* $GOOGLETEST_PATH

echo "Cleaning googletest"
rm -rf googletest-release-1.8.1
rm googletest-release-1.8.1.zip

