#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(ze_info
    SOURCE
    "include/ze_info/ze_info.hpp"
    "include/ze_info/capabilities.hpp"
    "include/ze_info/text_formatter.hpp"
    "include/ze_info/json_formatter.hpp"
    "src/ze_info.cpp"
    "src/capabilities.cpp"
    "src/text_formatter.cpp"
    "src/json_formatter.cpp"
)
target_link_libraries(ze_info_lib
    PUBLIC
    compute_samples::logging
    compute_samples::utils
    compute_samples::ze_utils
    Boost::program_options
    ze_loader
)

add_application(ze_info
    SOURCE
    "src/main.cpp"
)

add_application_test(ze_info
    SOURCE
    "test/main.cpp"
    "test/ze_info_unit_tests.cpp"
    "test/ze_info_system_tests.cpp"
)
