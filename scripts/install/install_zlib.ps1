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

echo "Downloading zlib"
wget -Uri https://zlib.net/zlib1211.zip -OutFile zlib1211.zip

echo "Extracting zlib"
. "C:\Program Files\7-Zip\7z.exe" x zlib1211.zip

echo "Installing zlib"
$rootPath = (Resolve-Path "$PSScriptRoot/../..").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$zlibPath = $installPath + "/zlib"

pushd
cd zlib-1.2.11
mkdir build | Out-Null
cd build

cmake .. -A x64 -DCMAKE_INSTALL_PREFIX="$zlibPath"
cmake --build . --target INSTALL --config Release
popd

echo "Cleaning zlib"
rm -R zlib-1.2.11
rm zlib1211.zip
