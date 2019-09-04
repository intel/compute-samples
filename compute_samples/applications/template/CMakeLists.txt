#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(template
    SOURCE
    "include/template/template.hpp"
    "src/template.cpp"
)
target_link_libraries(template_lib
    PUBLIC
    compute_samples::logging
    Boost::program_options
    compute_samples::ocl_utils
)
add_kernels(template_lib "template.cl")

add_application(template
    SOURCE
    "src/main.cpp"
)
install_kernels(template "template.cl")

add_application_test(template
    SOURCE
    "test/main.cpp"
    "test/template_unit_tests.cpp"
    "test/template_integration_tests.cpp"
    "test/template_system_tests.cpp"
)
install_kernels(template_tests "template.cl")
