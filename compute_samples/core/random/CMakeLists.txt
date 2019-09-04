#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(random
    SOURCE
    "include/random/random.hpp"
    "src/random.cpp"
)
target_link_libraries(random
    PUBLIC
    compute_samples::logging
)

add_core_library_test(random
    SOURCE
    "test/main.cpp"
    "test/random_unit_tests.cpp"
)
