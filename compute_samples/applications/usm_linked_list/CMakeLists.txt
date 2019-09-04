#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(usm_linked_list
    SOURCE
    "include/usm_linked_list/usm_linked_list.hpp"
    "src/usm_linked_list.cpp"
)
target_link_libraries(usm_linked_list_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::ocl_utils
    compute_samples::boost_intel
)
add_kernels(usm_linked_list_lib "usm_linked_list.cl")

add_application(usm_linked_list
    SOURCE
    "src/main.cpp"
)
install_kernels(usm_linked_list "usm_linked_list.cl")

add_application_test(usm_linked_list
    SOURCE
    "test/main.cpp"
    "test/usm_linked_list_integration_tests.cpp"
    "test/usm_linked_list_system_tests.cpp"
)
install_kernels(usm_linked_list_tests "usm_linked_list.cl")
