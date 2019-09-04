#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(ocl_utils
    SOURCE
    "include/ocl_utils/ocl_utils.hpp"
    "include/ocl_utils/unified_shared_memory.hpp"
    "src/ocl_utils.cpp"
    "src/unified_shared_memory.cpp"
)
target_link_libraries(ocl_utils
    PUBLIC
    compute_samples::logging
    compute_samples::utils
    compute_samples::boost_intel
    compute_samples::ocl_entrypoints
    OpenCL::OpenCL
)

add_core_library_test(ocl_utils
    SOURCE
    "test/main.cpp"
    "test/ocl_utils_unit_tests.cpp"
    "test/ocl_utils_integration_tests.cpp"
)
