#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(ocl_entrypoints
    SOURCE
    "include/ocl_entrypoints/loader.hpp"
    "include/ocl_entrypoints/cl_intel_aubs.h"
    "src/loader.cpp"
    "src/cl_intel_unified_shared_memory.cpp"
    "src/cl_intel_aubs.cpp"
)
target_link_libraries(ocl_entrypoints
    PUBLIC
    OpenCL::OpenCL
)
