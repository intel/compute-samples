#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading googletest"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri https://github.com/google/googletest/archive/release-1.8.1.zip -OutFile googletest-release-1.8.1.zip

echo "Extracting googletest"
. "C:\Program Files\7-Zip\7z.exe" x googletest-release-1.8.1.zip

echo "Installing googletest"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$googletestPath = $installPath + "/googletest"

mkdir $googletestPath | Out-Null

cp -Recurse googletest-release-1.8.1/* "$googletestPath"

echo "Cleaning googletest"
rm -R googletest-release-1.8.1
rm googletest-release-1.8.1.zip
