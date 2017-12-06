/*
 * Copyright(c) 2017 Intel Corporation
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

#include "subgroups_visualization/subgroups_visualization.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/record_ostream.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext_intel.h>

#include "image/image.hpp"
#include "timer/timer.hpp"

namespace compute_samples {

void SubgroupsVisualizationApplication::run(
    std::vector<std::string> &command_line, src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();

  if (device.supports_extension("cl_intel_subgroups")) {
    BOOST_LOG(logger) << "cl_intel_subgroups is supported.";
  }
  if (device.supports_extension("cl_intel_required_subgroup_size")) {
    BOOST_LOG(logger) << "cl_intel_required_subgroup_size is supported.";
  }
  if (!(device.supports_extension("cl_khr_subgroups") ||
        device.supports_extension("cl_intel_subgroups"))) {
    throw std::domain_error("This tutorial requires the cl_khr_subgroups or "
                            "cl_intel_subgroups extension.");
  }

  compute::context context(device);
  compute::command_queue queue(context, device);

  run_subgroups_visualization(args, context, queue, logger);
}

SubgroupsVisualizationApplication::Arguments
SubgroupsVisualizationApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "Show this help text.");
  options("kernel,k", po::value<std::string>(&args.kernel_path)
                          ->default_value("subgroups_visualization_kernel.cl"),
          "Kernel Filename.");
  options("output,o",
          po::value<std::string>(&args.output)->default_value("output.bmp"),
          "Output Bitmap.");
  options("global_size,g",
          po::value<size_t>(&args.global_size)->default_value(256),
          "Global Work Size.");
  options("local_size,l",
          po::value<size_t>(&args.local_size)->default_value(128),
          "Local Work Size");

  po::positional_options_description p;
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

void SubgroupsVisualizationApplication::run_subgroups_visualization(
    const SubgroupsVisualizationApplication::Arguments &args,
    compute::context &context, compute::command_queue &queue,
    src::logger &logger) const {
  Timer timer_total(logger);

  ImageBMP8Bit image_file(args.global_size, args.global_size);

  compute::program program =
      compute::program::create_with_source_file(args.kernel_path, context);
  try {
    program.build("-cl-std=CL2.0");
  } catch (compute::opencl_error &) {
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:" << std::endl
                      << program.build_log();
  }
  compute::kernel kernel = program.create_kernel("Visualize");

  compute::image_format format(CL_R, CL_UNSIGNED_INT8);
  compute::image2d image(context, image_file.width(), image_file.height(),
                         format, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR);

  kernel.set_args(image);

  // By default, the global work size is setup so there is one work item
  // for each image element, and the local work size is fixed to describe
  // a one-dimensional 128x1 work group.  You can experiment with different
  // global work sizes and local work sizes to see how this affects the
  // mapping of global IDs (AKA image elements) to subgroup IDs.  Note that
  // the kernel was compiled as an OpenCL 2.0 kernel and hence supports
  // non-uniform work groups, so the local work size need not evenly divide
  // the global work size.
  compute::extents<2> globalSize{image.width(), image.height()};
  compute::extents<2> localSize{args.local_size, 1};
  queue.enqueue_nd_range_kernel(kernel, compute::dim(0, 0), globalSize,
                                localSize);

  BOOST_LOG(logger) << "Global size is: " << globalSize[0] << "x"
                    << globalSize[1];
  BOOST_LOG(logger) << "Local size is: " << localSize[0] << "x" << localSize[1];
  BOOST_LOG(logger) << "Subgroup size is: "
                    << kernel.get_work_group_info<size_t>(
                           queue.get_device(),
                           CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);

  size_t row_pitch = 0;
  size_t slice_pitch = 0;
  unsigned char *data = static_cast<unsigned char *>(queue.enqueue_map_image(
      image, CL_MAP_READ, compute::dim(0, 0),
      compute::dim(image.width(), image.height()), row_pitch, slice_pitch));

  image_file.write(args.output, data);

  queue.enqueue_unmap_image(image, data);

  timer_total.print("Total");
}

} // namespace compute_samples
