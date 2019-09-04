#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading libpng"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri https://prdownloads.sourceforge.net/libpng/lpng1637.zip -OutFile lpng1637.zip -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::InternetExplorer

echo "Extracting libpng"
. "C:\Program Files\7-Zip\7z.exe" x lpng1637.zip

echo "Installing libpng"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$libpngPath = $installPath + "/libpng"
$zlibPath = $installPath + "/zlib"
$zlibIncludePath = $zlibPath + "/include"
$zlibLibPath = $zlibPath + "/lib/zlibstatic.lib"

pushd
cd lpng1637
mkdir build | Out-Null
cd build

cmake .. -A x64 -DCMAKE_INSTALL_PREFIX="$libpngPath" -DPNG_BUILD_ZLIB=ON -DZLIB_INCLUDE_DIR="$zlibIncludePath" -DZLIB_LIBRARY="$zlibLibPath" -DPNG_SHARED=OFF
cmake --build . --target INSTALL --config Release
popd

echo "Cleaning libpng"
rm -R lpng1637
rm lpng1637.zip
