/*
 * Copyright(c) 2018 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include <boost/log/sources/record_ostream.hpp>

#include "timer/timer.hpp"

namespace compute_samples {

const char *VmeInteropApplication::vme_extension_msg =
    "The selected device doesn't support device-side motion estimation.";
const char *VmeInteropApplication::vaapi_extension_msg =
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
    std::vector<std::string> &command_line, src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  Timer timer_total(logger);

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();

  if (!device.supports_extension(
          "cl_intel_device_side_avc_motion_estimation")) {
    BOOST_LOG(logger) << vme_extension_msg;
    return Status::SKIP;
  }

  if (!device.supports_extension("cl_intel_va_api_media_sharing")) {
    BOOST_LOG(logger) << vaapi_extension_msg;
    return Status::SKIP;
  }

  BOOST_LOG(logger) << "Input yuv path: " << args.input_yuv_path;
  BOOST_LOG(logger) << "Frame size: " << args.width << "x" << args.height
                    << " pixels";

  run_os_specific_implementation(command_line, args, device, logger);

  timer_total.print("Total");
  return Status::OK;
}
} // namespace compute_samples
