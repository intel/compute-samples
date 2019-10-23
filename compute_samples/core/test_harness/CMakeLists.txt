#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(test_harness
    SOURCE
    "include/test_harness/test_harness.hpp"
    "src/test_harness.cpp"
)
target_link_libraries(test_harness
    PUBLIC
    compute_samples::logging
    GTest::GTest
    Boost::program_options
)

add_core_library_test(test_harness
    SOURCE
    "test/main.cpp"
    "test/test_harness_unit_tests.cpp"
)