#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(logging
    SOURCE
    "include/logging/logging.hpp"
    "src/logging.cpp"
)
target_link_libraries(logging
    PUBLIC
    Boost::log
    Boost::program_options
)

add_core_library_test(logging
    SOURCE
    "test/main.cpp"
    "test/logging_unit_tests.cpp"
)
