#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

if(WITH_OCL)
    add_subdirectory(test_cl_unified_shared_memory)
    add_subdirectory(test_cl_visa_injection_rt_basic)
    add_subdirectory(test_cl_command_queue_families)
endif()
