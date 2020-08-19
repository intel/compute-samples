/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_utils/ze_utils.hpp"
#include "gtest/gtest.h"
#include <stdexcept>

TEST(throw_if_failed, ExceptionIsNotThrownWhenSuccessIsPassed) {
  EXPECT_NO_THROW(
      compute_samples::throw_if_failed(ZE_RESULT_SUCCESS, "message"));
}

TEST(throw_if_failed, ExceptionIsThrownWhenErrorIsPassed) {
  EXPECT_THROW(compute_samples::throw_if_failed(
                   ZE_RESULT_ERROR_INVALID_ARGUMENT, "message"),
               std::runtime_error);
}
