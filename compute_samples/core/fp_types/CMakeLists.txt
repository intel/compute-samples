#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(fp_types
    SOURCE
    "include/fp_types/common.hpp"
    "include/fp_types/half.hpp"
    "src/half.cpp"
)
target_link_libraries(fp_types
    PUBLIC
    compute_samples::logging
    compute_samples::ocl_utils
)

add_core_library_test(fp_types
    SOURCE
    "test/main.cpp"
    "test/half_unit_tests.cpp"
)
