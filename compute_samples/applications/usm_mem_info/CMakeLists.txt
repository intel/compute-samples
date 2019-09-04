#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(usm_mem_info
    SOURCE
    "include/usm_mem_info/usm_mem_info.hpp"
    "src/usm_mem_info.cpp"
)
target_link_libraries(usm_mem_info_lib
    PUBLIC
    compute_samples::logging
    Boost::program_options
    compute_samples::ocl_utils
    compute_samples::boost_intel
)

add_application(usm_mem_info
    SOURCE
    "src/main.cpp"
)

add_application_test(usm_mem_info
    SOURCE
    "test/main.cpp"
    "test/usm_mem_info_system_tests.cpp"
)
