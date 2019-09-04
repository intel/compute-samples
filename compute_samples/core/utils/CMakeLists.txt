#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(utils
    SOURCE
    "include/utils/utils.hpp"
    "src/utils.cpp"
)
target_link_libraries(utils
    PUBLIC
    compute_samples::logging
)

add_core_library_test(utils
    SOURCE
    "test/main.cpp"
    "test/utils_unit_tests.cpp"
    "test/utils_integration_tests.cpp"
)
install_resources(utils_tests
    FILES
    "test/binary_file.bin"
)
