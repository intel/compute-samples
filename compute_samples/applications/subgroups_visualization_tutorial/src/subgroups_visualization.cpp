/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "subgroups_visualization/subgroups_visualization.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext.h>

#include "image/image.hpp"
#include "timer/timer.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {

Application::Status SubgroupsVisualizationApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

  const compute::device device = compute::system::default_device();
  LOG_INFO << "OpenCL device: " << device.name();

  if (device.supports_extension("cl_intel_subgroups")) {
    LOG_INFO << "cl_intel_subgroups is supported.";
  }
  if (device.supports_extension("cl_intel_required_subgroup_size")) {
    LOG_INFO << "cl_intel_required_subgroup_size is supported.";
  }
  if (!(device.supports_extension("cl_khr_subgroups") ||
        device.supports_extension("cl_intel_subgroups"))) {
    LOG_ERROR << "This tutorial requires the cl_khr_subgroups or "
                 "cl_intel_subgroups extension.";
    return Status::SKIP;
  }

  compute::context context(device);
  compute::command_queue queue(context, device);

  run_subgroups_visualization(args, context, queue);
  return Status::OK;
}

SubgroupsVisualizationApplication::Arguments
SubgroupsVisualizationApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "Show this help text.");
  options("kernel,k",
          po::value<std::string>(&args.kernel_path)
              ->default_value("subgroups_visualization_kernel.cl"),
          "Kernel Filename.");
  options("output,o",
          po::value<std::string>(&args.output)->default_value("output.bmp"),
          "Output Bitmap.");
  options("global_size,g",
          po::value<int>(&args.global_size)->default_value(256),
          "Global Work Size.");
  options("local_size,l", po::value<int>(&args.local_size)->default_value(128),
          "Local Work Size");

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

void SubgroupsVisualizationApplication::run_subgroups_visualization(
    const SubgroupsVisualizationApplication::Arguments &args,
    compute::context &context, compute::command_queue &queue) const {
  Timer timer_total;

  ImageBMP8Bit image_file(args.global_size, args.global_size);

  compute::program program =
      build_program(context, args.kernel_path, "-cl-std=CL2.0");
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
  compute::extents<2> global_size{image.width(), image.height()};
  compute::extents<2> local_size{static_cast<size_t>(args.local_size), 1};
  queue.enqueue_nd_range_kernel(kernel, compute::dim(0, 0), global_size,
                                local_size);

  LOG_INFO << "Global size is: " << global_size[0] << "x" << global_size[1];
  LOG_INFO << "Local size is: " << local_size[0] << "x" << local_size[1];
  LOG_INFO << "Subgroup size is: "
           << kernel.get_work_group_info<size_t>(
                  queue.get_device(),
                  CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);

  size_t row_pitch = 0;
  size_t slice_pitch = 0;
  uint8_t *data = static_cast<uint8_t *>(queue.enqueue_map_image(
      image, CL_MAP_READ, compute::dim(0, 0),
      compute::dim(image.width(), image.height()), row_pitch, slice_pitch));

  image_file.write(args.output, data);

  queue.enqueue_unmap_image(image, data);

  timer_total.print("Total");
}

} // namespace compute_samples
