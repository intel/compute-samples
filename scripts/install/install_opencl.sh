#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
OPENCL_HEADERS_HASH="23710f1b99186065c1768fc3098ba681adc0f253"
wget "https://github.com/KhronosGroup/OpenCL-Headers/archive/$OPENCL_HEADERS_HASH.zip" -O "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Extracting OpenCL Headers"
unzip -q "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Installing OpenCL Headers"
ROOT_PATH=`pwd`
INSTALL_PATH="$ROOT_PATH/third_party"
OPENCL_PATH="$INSTALL_PATH/opencl"
OPENCL_INCLUDE_PATH="$OPENCL_PATH/include"

mkdir -p $OPENCL_INCLUDE_PATH
mkdir -p $OPENCL_LIB_PATH
cp -R "OpenCL-Headers-$OPENCL_HEADERS_HASH/CL" "$OPENCL_INCLUDE_PATH"

echo "Cleaning OpenCL Headers"
rm -rf "OpenCL-Headers-$OPENCL_HEADERS_HASH"
rm "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Downloading OpenCL ICD"
OPENCL_ICD_HASH="5f8249691ec8c25775789498951f8e9eb62c201d"
wget "https://github.com/KhronosGroup/OpenCL-ICD-Loader/archive/$OPENCL_ICD_HASH.zip" -O "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

echo "Extracting OpenCL ICD"
unzip -q "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

echo "Installing OpenCL ICD"
pushd .
cd "OpenCL-ICD-Loader-$OPENCL_ICD_HASH"
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DOPENCL_ICD_LOADER_HEADERS_DIR="$OPENCL_INCLUDE_PATH" -DCMAKE_INSTALL_PREFIX="$OPENCL_PATH"
cmake --build . --target install
popd

echo "Cleaning OpenCL ICD"
rm -rf "OpenCL-ICD-Loader-$OPENCL_ICD_HASH"
rm "OpenCL-ICD-Loader-$OPENCL_ICD_HASH.zip"

