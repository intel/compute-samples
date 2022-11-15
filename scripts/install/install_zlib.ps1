#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading zlib"
wget -Uri https://zlib.net/zlib1213.zip -OutFile zlib1213.zip

echo "Extracting zlib"
. "C:\Program Files\7-Zip\7z.exe" x zlib1213.zip

echo "Installing zlib"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$zlibPath = $installPath + "/zlib"

pushd
cd zlib-1.2.13
mkdir build | Out-Null
cd build

cmake .. -A x64 -DCMAKE_INSTALL_PREFIX="$zlibPath"
cmake --build . --target INSTALL --config Release
popd

echo "Cleaning zlib"
rm -R zlib-1.2.13
rm zlib1213.zip
