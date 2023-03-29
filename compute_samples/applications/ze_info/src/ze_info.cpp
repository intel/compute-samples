/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/ze_info.hpp"

#include "ze_info/capabilities.hpp"
#include "ze_info/text_formatter.hpp"
#include "ze_info/json_formatter.hpp"

#include "ze_api.h"
#include "zet_api.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "logging/logging.hpp"
#include "ze_utils/ze_utils.hpp"

namespace compute_samples {

ZeInfoApplication::Arguments ZeInfoApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("output", po::value<std::string>(&args.output),
          "path to output JSON file");

  po::positional_options_description p;

  po::variables_map vm;
  po::store(
      po::command_line_parser(command_line).options(desc).positional(p).run(),
      vm);

  if (vm.count("help") != 0u) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);
  return args;
}

Application::Status
ZeInfoApplication::run_implementation(std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

  const auto result = zeInit(0);
  throw_if_failed(result, "zeInit");
  LOG_DEBUG << "Drivers initialized";

  const std::vector<ze_driver_handle_t> drivers = get_drivers();
  const std::vector<DriverCapabilities> capabilities =
      get_drivers_capabilities(drivers);
  LOG_INFO << "Level Zero info:\n"
           << drivers_capabilities_to_text(capabilities, 0);

  if (!args.output.empty()) {
    LOG_DEBUG << "Saving output to: " << args.output;
    write_capabilities_to_json(capabilities, args.output);
  }

  return Status::OK;
}

} // namespace compute_samples
