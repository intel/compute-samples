#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(usm_queries
    SOURCE
    "include/usm_queries/usm_queries.hpp"
    "src/usm_queries.cpp"
)
target_link_libraries(usm_queries_lib
    PUBLIC
    compute_samples::logging
    Boost::program_options
    compute_samples::ocl_utils
    compute_samples::boost_intel
)

add_application(usm_queries
    SOURCE
    "src/main.cpp"
)

add_application_test(usm_queries
    SOURCE
    "test/main.cpp"
    "test/usm_queries_unit_tests.cpp"
    "test/usm_queries_system_tests.cpp"
)
