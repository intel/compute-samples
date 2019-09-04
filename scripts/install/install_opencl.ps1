#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
$openclHeadersHash = "0d5f18c6e7196863bc1557a693f1509adfcee056"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri "https://github.com/KhronosGroup/OpenCL-Headers/archive/$openclHeadersHash.zip" -OutFile "OpenCL-Headers-$openclHeadersHash.zip"

echo "Extracting OpenCL Headers"
. "C:\Program Files\7-Zip\7z.exe" x "OpenCL-Headers-$openclHeadersHash.zip"

echo "Installing OpenCL Headers"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$openclPath = $installPath + "/opencl"
$openclIncludePath = $openclPath + "/include"
$openclLibPath = $openclPath + "/lib"

cp -Recurse "OpenCL-Headers-$openclHeadersHash/CL" "$openclIncludePath/CL"

echo "Cleaning OpenCL Headers"
rm -R "OpenCL-Headers-$openclHeadersHash"
rm "OpenCL-Headers-$openclHeadersHash.zip"

echo "Downloading OpenCL ICD"
$openclICDHash = "b342ff7b7f70a4b3f2cfc53215af8fa20adc3d86"
wget -Uri "https://github.com/KhronosGroup/OpenCL-ICD-Loader/archive/$openclICDHash.zip" -OutFile "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Extracting OpenCL ICD"
. "C:\Program Files\7-Zip\7z.exe" x "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Installing OpenCL ICD"
pushd
cd "OpenCL-ICD-Loader-$openclICDHash"
mkdir build | Out-Null
cd build

cmake .. -A x64 -DOPENCL_INCLUDE_DIRS="$openclIncludePath" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="$openclLibPath" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="$openclLibPath"
cmake --build . --config Release
popd

echo "Cleaning OpenCL ICD"
rm -R "OpenCL-ICD-Loader-$openclICDHash"
rm "OpenCL-ICD-Loader-$openclICDHash.zip"
