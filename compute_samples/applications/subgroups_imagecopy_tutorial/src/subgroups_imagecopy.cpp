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

#include "subgroups_imagecopy/subgroups_imagecopy.hpp"

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

Application::Status SubgroupsImageCopyApplication::run_implementation(
    std::vector<std::string> &command_line, src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();

  if (device.supports_extension("cl_intel_subgroups")) {
    BOOST_LOG(logger) << "cl_intel_subgroups is supported.";
  }
  if (device.supports_extension("cl_intel_media_block_io")) {
    BOOST_LOG(logger) << "cl_intel_media_block_io is supported.";
  }
  if (!(device.supports_extension("cl_intel_subgroups"))) {
    BOOST_LOG(logger)
        << "This tutorial requires the cl_intel_subgroups extension.";
    return Status::SKIP;
  }

  compute::context context(device);
  compute::command_queue queue(context, device);

  run_subgroups_imagecopy(args, context, queue, logger);
  return Status::OK;
}

SubgroupsImageCopyApplication::Arguments
SubgroupsImageCopyApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "Show this help text.");
  options("kernel,k",
          po::value<std::string>(&args.kernel_path)
              ->default_value("subgroups_imagecopy_kernel.cl"),
          "Kernel Filename.");
  options("input,i",
          po::value<std::string>(&args.input)->default_value("input.bmp"),
          "Input Bitmap.");
  options("output,o",
          po::value<std::string>(&args.output)->default_value("output.bmp"),
          "Output Bitmap.");
  options("iterations,n", po::value<int>(&args.iterations)->default_value(1000),
          "Number of iterations to run.");

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

void SubgroupsImageCopyApplication::run_subgroups_imagecopy(
    const SubgroupsImageCopyApplication::Arguments &args,
    compute::context &context, compute::command_queue &queue,
    src::logger &logger) const {
  Timer timer_total(logger);

  ImageBMP8Bit image(args.input);

  BOOST_LOG(logger) << "Image Dimensions: " << image.width() << "x"
                    << image.height();

  compute::program program =
      compute::program::create_with_source_file(args.kernel_path, context);
  try {
    program.build("-cl-std=CL2.0");
  } catch (compute::opencl_error &) {
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:" << std::endl
                      << program.build_log();
    throw;
  }
  compute::kernel kernel = program.create_kernel("ImageCopy");

  compute::image_format format(CL_R, CL_UNSIGNED_INT8);
  compute::image2d outputImage(context, image.width(), image.height(), format,
                               CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR);
  compute::image2d inputImage(context, image.width(), image.height(), format,
                              CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                              image.raw_data());

  kernel.set_args(outputImage, inputImage);

  compute::extents<2> globalSize{static_cast<size_t>(image.width() / 4),
                                 static_cast<size_t>(image.height())};
  compute::extents<2> localSize{32, 1};

  BOOST_LOG(logger) << "Executing " << args.iterations << " iterations.";
  BOOST_LOG(logger) << "Global size is: " << globalSize[0] << "x"
                    << globalSize[1];
  BOOST_LOG(logger) << "Local size is: " << localSize[0] << "x" << localSize[1];
  BOOST_LOG(logger) << "Subgroup size is: "
                    << kernel.get_work_group_info<size_t>(
                           queue.get_device(),
                           CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);

  Timer timer_execution(logger);

  for (int i = 0; i < args.iterations; i++) {
    // By default, the global work size is setup so there is one work item
    // for every four image elements, and the local work size is fixed to
    // describe a one-dimensional 32x1 work group.  It may be possible to
    // improve performance by processing even more than four image elements
    // per work item!
    queue.enqueue_nd_range_kernel(kernel, compute::dim(0, 0), globalSize,
                                  localSize);
  }

  queue.finish();

  timer_execution.print("Image Copy Execution Time");

  size_t row_pitch = 0;
  size_t slice_pitch = 0;
  uint8_t *data = static_cast<uint8_t *>(queue.enqueue_map_image(
      outputImage, CL_MAP_READ, compute::dim(0, 0),
      compute::dim(outputImage.width(), outputImage.height()), row_pitch,
      slice_pitch));

  image.write(args.output, data);

  queue.enqueue_unmap_image(outputImage, data);

  timer_total.print("Total");
}

} // namespace compute_samples
