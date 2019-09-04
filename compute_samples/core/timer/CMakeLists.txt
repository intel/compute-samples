#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(timer
    SOURCE
    "include/timer/timer.hpp"
    "src/timer.cpp"
)
target_link_libraries(timer
    PUBLIC
    Boost::timer
    compute_samples::logging
)
