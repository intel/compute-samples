/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include "commands_aggregation/commands_aggregation.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(CommandsAggregationApplication, ReturnsOkStatus) {
  compute_samples::CommandsAggregationApplication application;
  std::vector<std::string> command_line = {};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(CommandsAggregationApplication,
       ComputesExpectedResultsUsingSingleQueue) {
  compute_samples::CommandsAggregationApplication app;
  const int global_work_size = 128;

  const std::vector<uint32_t> output =
      app.run_workloads_in_order(global_work_size);

  const int number_of_kernels = 10;
  const int number_of_iterations = 1000000;
  const std::vector<uint32_t> reference(number_of_kernels * global_work_size,
                                        number_of_iterations);
  EXPECT_EQ(output, reference);
}

HWTEST(CommandsAggregationApplication,
       ComputesExpectedResultsUsingMultipleQueues) {
  compute_samples::CommandsAggregationApplication app;
  const int global_work_size = 128;

  const std::vector<uint32_t> output =
      app.run_workloads_out_of_order(global_work_size);

  const int number_of_kernels = 10;
  const int number_of_iterations = 1000000;
  const std::vector<uint32_t> reference(number_of_kernels * global_work_size,
                                        number_of_iterations);
  EXPECT_EQ(output, reference);
}
