#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(offline_compiler
    SOURCE
    "include/offline_compiler/offline_compiler.hpp"
    "src/offline_compiler.cpp"
)
target_link_libraries(offline_compiler
    PUBLIC
    compute_samples::logging
    compute_samples::utils
)

add_core_library_test(offline_compiler
    SOURCE
    "test/main.cpp"
    "test/offline_compiler_integration_tests.cpp"
)