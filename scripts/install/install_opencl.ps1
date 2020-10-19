#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading OpenCL Headers"
$openclHeadersHash = "9fac4e9866a961f66bdd72fa2bff50145512f972"
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
$openclICDHash = "b3b483303b160db080857288c1b53e8152f62b98"
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
