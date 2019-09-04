/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "subgroups_imagecopy/subgroups_imagecopy.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(SubgroupsImageCopyIntegrationTests, BasicProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(
                context, "subgroups_imagecopy_kernel.cl", "-cl-std=CL2.0"));
}

HWTEST(SubgroupsImageCopyIntegrationTests, OptimizedProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(
                context, "subgroups_imagecopy_solution.cl", "-cl-std=CL2.0"));
}
