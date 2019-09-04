/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ocl_entrypoints/cl_intel_aubs.h"
#include "ocl_entrypoints/loader.hpp"

namespace cs = compute_samples;

CL_API_ENTRY cl_int CL_API_CALL clEnqueueVerifyMemoryINTEL(
    cl_command_queue command_queue, const void *actual_data,
    const void *expected_data, size_t size_of_comparison,
    cl_uint comparison_mode) CL_API_SUFFIX__VERSION_1_0 {
  const auto e = cs::load_entrypoint<clEnqueueVerifyMemoryINTEL_fn>(
      command_queue, "clEnqueueVerifyMemoryINTEL");
  return e(command_queue, actual_data, expected_data, size_of_comparison,
           comparison_mode);
}

CL_API_ENTRY cl_int CL_API_CALL clAddCommentINTEL(
    cl_platform_id platform, const char *comment) CL_API_SUFFIX__VERSION_2_1 {
  const auto e =
      cs::load_entrypoint<clAddCommentINTEL_fn>(platform, "clAddCommentINTEL");
  return e(platform, comment);
}
