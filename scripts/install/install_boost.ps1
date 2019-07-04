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

echo "Downloading Boost"
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
wget -Uri https://prdownloads.sourceforge.net/boost/boost/1.64.0/boost_1_64_0.zip -OutFile boost_1_64_0.zip -UserAgent [Microsoft.PowerShell.Commands.PSUserAgent]::InternetExplorer

echo "Extracting boost"
. "C:\Program Files\7-Zip\7z.exe" x boost_1_64_0.zip

echo "Installing Boost"
$rootPath = (Resolve-Path ".").ToString()
$installPath = $rootPath + "/third_party"
mkdir $installPath -Force
$boostPath = $installPath + "/boost"
$jobs = 4

pushd
cd boost_1_64_0

./bootstrap
./b2 address-model=64 install --with-program_options --with-timer --with-chrono --with-log --with-system --prefix="$boostPath" -j"$jobs" -d0

popd

echo "Cleaning Boost"
rm -R boost_1_64_0 -Force
rm boost_1_64_0.zip
