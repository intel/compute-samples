/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "vme_search/vme_search.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(VmeSearchIntegrationTests, BasicSearchProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "vme_basic_search.cl"));
}

HWTEST(VmeSearchIntegrationTests, CostHeuristicsSearchProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(), compute_samples::build_program(
                                    context, "vme_cost_heuristics_search.cl"));
}

HWTEST(VmeSearchIntegrationTests, LargerSearchProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "vme_larger_search.cl"));
}
