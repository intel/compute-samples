/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CL_INTEL_AUBS_H
#define CL_INTEL_AUBS_H

#include <CL/cl.h>

#define CL_COMPARE_EQUAL 0x00000
#define CL_COMPARE_NOT_EQUAL 0x00001

typedef CL_API_ENTRY cl_int(CL_API_CALL *clEnqueueVerifyMemoryINTEL_fn)(
    cl_command_queue command_queue, const void *actual_data,
    const void *expected_data, size_t size_of_comparison,
    cl_uint comparison_mode) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL clEnqueueVerifyMemoryINTEL(
    cl_command_queue command_queue, const void *actual_data,
    const void *expected_data, size_t size_of_comparison,
    cl_uint comparison_mode) CL_API_SUFFIX__VERSION_1_0;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clAddCommentINTEL_fn)(
    cl_platform_id platform, const char *comment) CL_API_SUFFIX__VERSION_2_1;

extern CL_API_ENTRY cl_int CL_API_CALL clAddCommentINTEL(
    cl_platform_id platform, const char *comment) CL_API_SUFFIX__VERSION_2_1;

#endif
