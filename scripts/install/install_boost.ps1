#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading Boost"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.zip -OutFile boost_1_71_0.zip -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::InternetExplorer

echo "Extracting boost"
. "C:\Program Files\7-Zip\7z.exe" x boost_1_71_0.zip

echo "Installing Boost"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$boostPath = $installPath + "/boost"
$jobs = 4

pushd
cd boost_1_71_0

./bootstrap
./b2 address-model=64 install --with-program_options --with-timer --with-chrono --with-log --with-system --prefix="$boostPath" -j"$jobs" -d0

popd

echo "Cleaning Boost"
rm -R boost_1_71_0 -Force
rm boost_1_71_0.zip
