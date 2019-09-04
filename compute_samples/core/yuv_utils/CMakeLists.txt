#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(yuv_utils
    SOURCE
    "include/yuv_utils/yuv_utils.hpp"
    "src/yuv_utils.cpp"
)
target_link_libraries(yuv_utils
    PUBLIC
    compute_samples::image
)
