#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_test_suite(test_cl_unified_shared_memory
    SOURCE
    "include/test_cl_unified_shared_memory/common.hpp"
    "src/common.cpp"
    "src/main.cpp"
    "src/test_atomics.cpp"
    "src/test_device_info.cpp"
    "src/test_events.cpp"
    "src/test_indirect_access.cpp"
    "src/test_mem_advise.cpp"
    "src/test_mem_alloc.cpp"
    "src/test_mem_cpy.cpp"
    "src/test_mem_free.cpp"
    "src/test_mem_migrate.cpp"
    "src/test_mem_fill.cpp"
    "src/test_set_kernel_arg_mem.cpp"
)
target_link_libraries(test_cl_unified_shared_memory
    PUBLIC
    compute_samples::logging
    compute_samples::ocl_utils
    compute_samples::utils
)
add_kernels(test_cl_unified_shared_memory "test_cl_unified_shared_memory.cl")
install_kernels(test_cl_unified_shared_memory "test_cl_unified_shared_memory.cl")
