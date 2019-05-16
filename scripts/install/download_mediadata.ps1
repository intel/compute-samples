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

echo "Downloading mediadata"
wget -Uri https://software.intel.com/file/604709/download -OutFile yuv_samples.tgz

echo "Extracting mediadata"
. "C:\Program Files\7-Zip\7z.exe" e yuv_samples.tgz
. "C:\Program Files\7-Zip\7z.exe" x yuv_samples.tar

echo "Installing mediadata"
$rootPath = (Resolve-Path "$PSScriptRoot/../..").ToString()
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
