#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading mediadata"
wget https://software.intel.com/file/604709/download -O yuv_samples.tgz

echo "Extracting mediadata"
tar xf yuv_samples.tgz

echo "Installing mediadata"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/mediadata"
YUV_PATH="$INSTALL_PATH/external/yuv"

mkdir -p $YUV_PATH
cp -R yuv_samples/* $YUV_PATH

echo "Cleaning mediadata"
rm -rf yuv_samples
rm yuv_samples.tgz

