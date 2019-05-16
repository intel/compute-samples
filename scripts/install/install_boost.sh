#!/bin/bash

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
wget https://prdownloads.sourceforge.net/boost/boost/1.64.0/boost_1_64_0.tar.gz -O boost_1_64_0.tar.gz

echo "Extracting Boost"
tar xvf boost_1_64_0.tar.gz

echo "Installing Boost"
SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
INSTALL_PATH="$SCRIPT_PATH/../../third_party"
BOOST_PATH="$INSTALL_PATH/boost"
JOBS="${JOBS:-4}"

pushd .
cd boost_1_64_0

./bootstrap.sh
./b2 address-model=64 install --with-program_options --with-timer --with-chrono --with-log --with-system --prefix=$BOOST_PATH -j$JOBS
popd

echo "Cleaning Boost"
rm -rf boost_1_64_0
rm boost_1_64_0.tar.gz

