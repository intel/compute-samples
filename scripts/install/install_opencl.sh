#!/bin/bash

#
# Copyright(c) 2019 Intel Corporation
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

echo "Downloading OpenCL Headers"
OPENCL_HEADERS_HASH="de26592167b9fdea503885e40e8755393c56523d"
wget "https://github.com/KhronosGroup/OpenCL-Headers/archive/$OPENCL_HEADERS_HASH.zip" -O "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Extracting OpenCL Headers"
unzip -q "OpenCL-Headers-$OPENCL_HEADERS_HASH.zip"

echo "Installing OpenCL Headers"
SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
INSTALL_PATH="$SCRIPT_PATH/../../third_party"
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

