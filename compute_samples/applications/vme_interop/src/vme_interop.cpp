/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interop/vme_interop.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/compute/core.hpp>
#include <boost/compute/utility.hpp>

#include "timer/timer.hpp"
#include "logging/logging.hpp"

namespace compute_samples {

const char *VmeInteropApplication::vme_extension_msg_ =
    "The selected device doesn't support device-side motion estimation.";
const char *VmeInteropApplication::vaapi_extension_msg_ =
    "The selected device doesn't support VA API media sharing.";

VmeInteropApplication::Arguments VmeInteropApplication::parse_command_line(
    const std::vector<std::string> &command_line) {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("output-bmp,b",
          po::value<bool>(&args.output_bmp)
              ->default_value(false)
              ->implicit_value(true),
          "output to bmp images for each frame");
  options("input-yuv,i",
          po::value<std::string>(&args.input_yuv_path)
              ->default_value("goal_1280x720.yuv"),
          "path to input yuv file");
  options("output-yuv,o",
          po::value<std::string>(&args.output_yuv_path)
              ->default_value("output_goal_1280x720.yuv"),
          "path to output yuv with motion vectors");
  options("width,w", po::value<int>(&args.width)->default_value(1280),
          "width of input yuv");
  options("height,h", po::value<int>(&args.height)->default_value(720),
          "height of input yuv");
  options("frames,f", po::value<int>(&args.frames)->default_value(0),
          "number of frame to use for motion estimation (0 represents entire "
          "yuv sequence)");

  po::positional_options_description p;
  p.add("input-yuv", 1);
  p.add("output-yuv", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(command_line).options(desc).positional(p).run(),
      vm);

  if (vm.count("help")) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);
  return args;
}

Application::Status VmeInteropApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  Timer timer_total;

  const compute::device device = compute::system::default_device();
  LOG_INFO << "OpenCL device: " << device.name();

  if (!device.supports_extension(
          "cl_intel_device_side_avc_motion_estimation")) {
    LOG_ERROR << vme_extension_msg_;
    return Status::SKIP;
  }

  if (!device.supports_extension("cl_intel_va_api_media_sharing")) {
    LOG_ERROR << vaapi_extension_msg_;
    return Status::SKIP;
  }

  LOG_INFO << "Input yuv path: " << args.input_yuv_path;
  LOG_INFO << "Frame size: " << args.width << "x" << args.height << " pixels";

  run_os_specific_implementation(command_line, args, device);

  timer_total.print("Total");
  return Status::OK;
}
} // namespace compute_samples
