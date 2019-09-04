#!/bin/bash

#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

echo "Installing all dependencies"
SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$SCRIPT_PATH/install_googletest.sh
$SCRIPT_PATH/download_mediadata.sh
echo
echo "Please install remaining packages using the following command line:"
echo "apt-get install libpng-dev ocl-icd-opencl-dev libboost-all-dev libva-dev"

