#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(commands_aggregation
    SOURCE
    "include/commands_aggregation/commands_aggregation.hpp"
    "src/commands_aggregation.cpp"
)
target_link_libraries(commands_aggregation_lib
    PUBLIC
    compute_samples::timer
    compute_samples::ocl_utils
    compute_samples::utils
    compute_samples::logging
    Boost::program_options
)
add_kernels(commands_aggregation_lib "commands_aggregation.cl")

add_application(commands_aggregation
    SOURCE
    "src/main.cpp"
)
install_kernels(commands_aggregation "commands_aggregation.cl")

add_application_test(commands_aggregation
    SOURCE
    "test/main.cpp"
    "test/commands_aggregation_integration_tests.cpp"
    "test/commands_aggregation_system_tests.cpp"
)
install_kernels(commands_aggregation_tests "commands_aggregation.cl")
