#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_subdirectory(align_utils)
add_subdirectory(application)
add_subdirectory(boost_intel)
add_subdirectory(image)
add_subdirectory(timer)
add_subdirectory(version)
add_subdirectory(yuv_utils)
add_subdirectory(logging)
add_subdirectory(random)
add_subdirectory(utils)
add_subdirectory(core_template)
add_subdirectory(test_harness)

if(WITH_OCL)
    add_subdirectory(ocl_utils)
    add_subdirectory(ocl_entrypoints)
    add_subdirectory(fp_types)
endif()
if(WITH_L0)
    add_subdirectory(ze_utils)
endif()
add_subdirectory(offline_compiler)
add_subdirectory(va_utils)
