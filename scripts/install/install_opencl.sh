#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
OPENCL_HEADERS_HASH="0d5f18c6e7196863bc1557a693f1509adfcee056"
wget "https://github.com/KhronosGroup/OpenCL-Headers/archive/$OPENCL_HEADERS_HASH.zip" -O "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Extracting OpenCL Headers"
unzip -q "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Installing OpenCL Headers"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/third_party"
OPENCL_PATH="$INSTALL_PATH/opencl"
OPENCL_INCLUDE_PATH="$OPENCL_PATH/include"
OPENCL_LIB_PATH="$OPENCL_PATH/lib"

mkdir -p $OPENCL_INCLUDE_PATH
mkdir -p $OPENCL_LIB_PATH
cp -R "OpenCL-Headers-$OPENCL_HEADERS_HASH/CL" "$OPENCL_INCLUDE_PATH"

echo "Cleaning OpenCL Headers"
rm -rf "OpenCL-Headers-$OPENCL_HEADERS_HASH"
rm "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Downloading OpenCL ICD"
OPENCL_ICD_HASH="b342ff7b7f70a4b3f2cfc53215af8fa20adc3d86"
wget "https://github.com/KhronosGroup/OpenCL-ICD-Loader/archive/$OPENCL_ICD_HASH.zip" -O "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

echo "Extracting OpenCL ICD"
unzip -q "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

echo "Installing OpenCL ICD"
pushd .
cd "OpenCL-ICD-Loader-$OPENCL_ICD_HASH"
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DOPENCL_INCLUDE_DIRS="$OPENCL_INCLUDE_PATH"
cmake --build .

cp -R lib/* $OPENCL_LIB_PATH
popd

echo "Cleaning OpenCL ICD"
rm -rf "OpenCL-ICD-Loader-$OPENCL_ICD_HASH"
rm "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

