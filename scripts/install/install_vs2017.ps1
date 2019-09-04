#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Installing all dependencies"
. "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"
. "$PSScriptRoot/install_zlib"
. "$PSScriptRoot/install_libpng"
. "$PSScriptRoot/install_opencl"
. "$PSScriptRoot/install_googletest"
. "$PSScriptRoot/install_boost"
. "$PSScriptRoot/download_mediadata"
