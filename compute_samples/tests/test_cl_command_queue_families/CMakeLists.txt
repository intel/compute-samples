#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_test_suite(test_cl_command_queue_families
    SOURCE
    "src/test_cl_command_queue_families.cpp"
    "src/main.cpp"
)
target_link_libraries(test_cl_command_queue_families
    PUBLIC
    compute_samples::logging
    compute_samples::ocl_utils
    compute_samples::utils
)
add_kernels(test_cl_command_queue_families "test_cl_command_queue_families.cl")
install_kernels(test_cl_command_queue_families "test_cl_command_queue_families.cl")
