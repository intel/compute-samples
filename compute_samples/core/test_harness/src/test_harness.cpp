/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_harness/test_harness.hpp"
#include "logging/logging.hpp"
#include "gtest/gtest.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace compute_samples {
static bool allow_skips = false;

void init_test_harness() { allow_skips = false; }

void init_test_harness(const TestHarnessSettings settings) {
  allow_skips = settings.allow_skips;
}

void init_test_harness(std::vector<std::string> &command_line) {
  init_test_harness(test_harness_parse_command_line(command_line));
}

TestHarnessSettings
test_harness_parse_command_line(std::vector<std::string> &command_line) {
  TestHarnessSettings settings;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("allow-skips", po::bool_switch(&settings.allow_skips),
          "skip unsupported scenarios");

  po::parsed_options parsed = po::command_line_parser(command_line)
                                  .options(desc)
                                  .allow_unregistered()
                                  .run();
  po::variables_map vm;
  po::store(parsed, vm);
  po::notify(vm);

  command_line =
      po::collect_unrecognized(parsed.options, po::include_positional);
  return settings;
}

bool are_skips_allowed() { return allow_skips; }

} // namespace compute_samples
