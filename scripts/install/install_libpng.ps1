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

echo "Downloading libpng"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri https://prdownloads.sourceforge.net/libpng/lpng1630.zip -OutFile lpng1630.zip -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::InternetExplorer

echo "Extracting libpng"
. "C:\Program Files\7-Zip\7z.exe" x lpng1630.zip

echo "Installing libpng"
$rootPath = (Resolve-Path "$PSScriptRoot/../..").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$libpngPath = $installPath + "/libpng"
$zlibPath = $installPath + "/zlib"
$zlibIncludePath = $zlibPath + "/include"
$zlibLibPath = $zlibPath + "/lib/zlibstatic.lib"

pushd
cd lpng1630
mkdir build | Out-Null
cd build

cmake .. -A x64 -DCMAKE_INSTALL_PREFIX="$libpngPath" -DPNG_BUILD_ZLIB=ON -DZLIB_INCLUDE_DIR="$zlibIncludePath" -DZLIB_LIBRARY="$zlibLibPath" -DPNG_SHARED=OFF
cmake --build . --target INSTALL --config Release
popd

echo "Cleaning libpng"
rm -R lpng1630
rm lpng1630.zip
