/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "subgroups_visualization/subgroups_visualization.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

namespace compute = boost::compute;

HWTEST(SubgroupsVisualizationCopyIntegrationTests, WorkgroupProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(
                context, "subgroups_visualization_kernel.cl", "-cl-std=CL2.0"));
}

HWTEST(SubgroupsVisualizationCopyIntegrationTests, SubgroupProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(
      compute::program(),
      compute_samples::build_program(
          context, "subgroups_visualization_solution.cl", "-cl-std=CL2.0"));
}
