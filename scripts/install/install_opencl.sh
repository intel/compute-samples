#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
OPENCL_HEADERS_HASH="9fac4e9866a961f66bdd72fa2bff50145512f972"
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
OPENCL_ICD_HASH="b3b483303b160db080857288c1b53e8152f62b98"
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

