#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(usm_hello_world
    SOURCE
    "include/usm_hello_world/usm_hello_world.hpp"
    "src/usm_hello_world.cpp"
)
target_link_libraries(usm_hello_world_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::ocl_utils
    compute_samples::boost_intel
)
add_kernels(usm_hello_world_lib "usm_hello_world.cl")

add_application(usm_hello_world
    SOURCE
    "src/main.cpp"
)
install_kernels(usm_hello_world "usm_hello_world.cl")

add_application_test(usm_hello_world
    SOURCE
    "test/main.cpp"
    "test/usm_hello_world_integration_tests.cpp"
    "test/usm_hello_world_system_tests.cpp"
)
install_kernels(usm_hello_world_tests "usm_hello_world.cl")
