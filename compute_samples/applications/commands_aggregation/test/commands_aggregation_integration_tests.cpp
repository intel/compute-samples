/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "commands_aggregation/commands_aggregation.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

namespace compute = boost::compute;

HWTEST(CommandsAggregationIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "commands_aggregation.cl",
                                           "-cl-opt-disable"));
}
