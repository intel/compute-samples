#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(core_template
    SOURCE
    "include/core_template/core_template.hpp"
    "src/core_template.cpp"
)
target_link_libraries(core_template
    PUBLIC
    compute_samples::logging
)

add_core_library_test(core_template
    SOURCE
    "test/main.cpp"
    "test/core_template_unit_tests.cpp"
)