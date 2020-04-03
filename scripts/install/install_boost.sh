#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading Boost"
wget https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.gz -O boost_1_71_0.tar.gz

echo "Extracting Boost"
tar xf boost_1_71_0.tar.gz

echo "Installing Boost"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/third_party"
BOOST_PATH="$INSTALL_PATH/boost"
JOBS="${JOBS:-4}"

pushd .
cd boost_1_71_0

./bootstrap.sh
./b2 address-model=64 install --with-program_options --with-timer --with-chrono --with-log --with-system --prefix=$BOOST_PATH -j$JOBS -d0
popd

echo "Cleaning Boost"
rm -rf boost_1_71_0
rm boost_1_71_0.tar.gz

