#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Installing all dependencies"
. "$PSScriptRoot/install_zlib"
. "$PSScriptRoot/install_libpng"
. "$PSScriptRoot/install_opencl"
. "$PSScriptRoot/install_googletest"
. "$PSScriptRoot/install_boost"
. "$PSScriptRoot/install_level_zero"
. "$PSScriptRoot/download_mediadata"
