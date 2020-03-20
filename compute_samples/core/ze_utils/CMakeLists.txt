#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(ze_utils
    SOURCE
    "include/ze_utils/ze_utils.hpp"
    "src/ze_utils.cpp"
)
target_link_libraries(ze_utils
    PUBLIC
    compute_samples::logging
    ze_loader
)

add_core_library_test(ze_utils
    SOURCE
    "test/main.cpp"
    "test/ze_utils_unit_tests.cpp"
)
