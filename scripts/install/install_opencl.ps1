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

echo "Downloading OpenCL Headers"
$openclHeadersHash = "de26592167b9fdea503885e40e8755393c56523d"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri "https://github.com/KhronosGroup/OpenCL-Headers/archive/$openclHeadersHash.zip" -OutFile "OpenCL-Headers-$openclHeadersHash.zip"

echo "Extracting OpenCL Headers"
. "C:\Program Files\7-Zip\7z.exe" x "OpenCL-Headers-$openclHeadersHash.zip"

echo "Installing OpenCL Headers"
$rootPath = (Resolve-Path "$PSScriptRoot/../..").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$openclPath = $installPath + "/opencl"
$openclIncludePath = $openclPath + "/include"
$openclLibPath = $openclPath + "/lib"

cp -Recurse "OpenCL-Headers-$openclHeadersHash/CL" "$openclIncludePath/CL"

echo "Cleaning OpenCL Headers"
rm -R "OpenCL-Headers-$openclHeadersHash"
rm "OpenCL-Headers-$openclHeadersHash.zip"

echo "Downloading OpenCL ICD"
$openclICDHash = "b342ff7b7f70a4b3f2cfc53215af8fa20adc3d86"
wget -Uri "https://github.com/KhronosGroup/OpenCL-ICD-Loader/archive/$openclICDHash.zip" -OutFile "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Extracting OpenCL ICD"
. "C:\Program Files\7-Zip\7z.exe" x "OpenCL-ICD-Loader-$openclICDHash.zip"

echo "Installing OpenCL ICD"
pushd
cd "OpenCL-ICD-Loader-$openclICDHash"
mkdir build | Out-Null
cd build

cmake .. -A x64 -DOPENCL_INCLUDE_DIRS="$openclIncludePath" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="$openclLibPath" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="$openclLibPath"
cmake --build . --config Release
popd

echo "Cleaning OpenCL ICD"
rm -R "OpenCL-ICD-Loader-$openclICDHash"
rm "OpenCL-ICD-Loader-$openclICDHash.zip"
