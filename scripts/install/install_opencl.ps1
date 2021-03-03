#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
$openclHeadersHash = "23710f1b99186065c1768fc3098ba681adc0f253"
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

cp -Recurse "OpenCL-Headers-$openclHeadersHash/CL" "$openclIncludePath/CL"

echo "Cleaning OpenCL Headers"
rm -R "OpenCL-Headers-$openclHeadersHash"
rm "OpenCL-Headers-$openclHeadersHash.zip"

echo "Downloading OpenCL ICD"
$openclICDHash = "5f8249691ec8c25775789498951f8e9eb62c201d"
wget -Uri "https://github.com/KhronosGroup/OpenCL-ICD-Loader/archive/$openclICDHash.zip" -OutFile "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Extracting OpenCL ICD"
. "C:\Program Files\7-Zip\7z.exe" x "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Installing OpenCL ICD"
pushd
cd "OpenCL-ICD-Loader-$openclICDHash"
mkdir build | Out-Null
cd build

cmake .. -A x64 -DOPENCL_ICD_LOADER_HEADERS_DIR="$openclIncludePath" -DCMAKE_INSTALL_PREFIX="$openclPath"
cmake --build . --target install --config Release
popd

echo "Cleaning OpenCL ICD"
rm -R "OpenCL-ICD-Loader-$openclICDHash"
rm "OpenCL-ICD-Loader-$openclICDHash.zip"
