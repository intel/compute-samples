#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Downloading mediadata"
wget -Uri https://software.intel.com/file/604709/download -OutFile yuv_samples.tgz

echo "Extracting mediadata"
. "C:\Program Files\7-Zip\7z.exe" e yuv_samples.tgz
. "C:\Program Files\7-Zip\7z.exe" x yuv_samples.tar

echo "Installing mediadata"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/mediadata"
$externalPath = $installPath + "/external"
$yuvPath = $externalPath + "/yuv"

mkdir $externalPath | Out-Null
mkdir $yuvPath | Out-Null
cp -Recurse yuv_samples/* "$yuvPath"

echo "Cleaning mediadata"
rm -R yuv_samples
rm yuv_samples.tar
rm yuv_samples.tgz
