/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "vme_wpp/vme_wpp.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(VmeWppIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(), compute_samples::build_program(
                                    context, "vme_wpp.cl", "-cl-std=CL2.0"));
}
