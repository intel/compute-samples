/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "logging/logging.hpp"
#include "core_template/core_template.hpp"
#include "gtest/gtest.h"

namespace cs = compute_samples;

TEST(CoreTemplateUnitTests, NumbersAddedCorrectly) {
  LOG_DEBUG << "Test Started";
  EXPECT_EQ(4, cs::add_numbers(2, 2));
}
