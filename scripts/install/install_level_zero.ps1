#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading Level Zero"
wget -Uri https://github.com/oneapi-src/level-zero/archive/v1.6.2.zip -OutFile level-zero-1.6.2.zip

echo "Extracting Level Zero"
. "C:\Program Files\7-Zip\7z.exe" x level-zero-1.6.2.zip

echo "Installing Level Zero"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$levelZeroPath = $installPath + "/level_zero"

pushd
cd level-zero-1.6.2
mkdir build | Out-Null
cd build

cmake .. -A x64 -DCMAKE_INSTALL_PREFIX="$levelZeroPath"
cmake --build . --target INSTALL --config Release
popd

echo "Cleaning Level Zero"
rm -R level-zero-1.6.2
rm level-zero-1.6.2.zip
