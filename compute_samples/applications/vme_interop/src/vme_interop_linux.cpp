/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interop/vme_interop.hpp"
#include "va_utils/va_utils.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext_intel.h>

#include "align_utils/align_utils.hpp"

#include "boost/compute/intel/image/image2d_va.hpp"
#include "timer/timer.hpp"
#include "yuv_utils/yuv_utils.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

#include <CL/cl_va_api_media_sharing_intel.h>
#include <fcntl.h>

namespace au = compute_samples::align_utils;
namespace compute = boost::compute;

namespace compute_samples {
compute_samples::VAManager vamanager;
static void write_va_surface(const VADisplay va_display,
                             const VASurfaceID va_surface,
                             const PlanarImage &planar_image) {
  VAImage va_image;
  VAStatus status = vamanager.vaDeriveImage(va_display, va_surface, &va_image);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("write_va_surface failed");
  }

  uint8_t *ptr;
  status = vamanager.vaMapBuffer(va_display, va_image.buf,
                                 reinterpret_cast<void **>(&ptr));
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("write_va_surface failed");
  }

  // Write luma.

  size_t width = planar_image.get_width();
  size_t height = planar_image.get_height();
  size_t pitch_y = planar_image.get_pitch_y();
  size_t pitch_u = planar_image.get_pitch_u();
  size_t pitch_v = planar_image.get_pitch_v();
  const uint8_t *y_plane = planar_image.get_y();

  for (size_t i = 0; i < height; ++i) {
    std::copy(y_plane + (i * pitch_y), y_plane + (i * pitch_y) + width,
              ptr + va_image.offsets[0] + i * va_image.pitches[0]);
  }

  // Replicate the trailing row of pixels to fill-in any remaining rows
  // in the VA image. VA_API images heights have stricter alignment
  // requirement and may be larger than the actual image height, so
  // we need to replicate the image pixel rows to match the VME unit's
  // behavior to handle out-of-bound references.
  for (size_t i = height; i < va_image.height; ++i) {
    std::copy(y_plane + ((height - 1) * pitch_y),
              y_plane + ((height - 1) * pitch_y) + width,
              ptr + va_image.offsets[0] + i * va_image.pitches[0]);
  }

  // Write chroma.

  const uint8_t *u_plane = planar_image.get_u();
  const uint8_t *v_plane = planar_image.get_v();

  uint8_t *chroma_dst = ptr + va_image.offsets[1];

  for (size_t j = 0; j < height / 2; j += 1) {
    uint8_t *chroma_tmp = chroma_dst;
    for (size_t k = 0; k < width / 2; k += 1) {
      chroma_dst[0] = *(u_plane + (j * pitch_u) + k);
      chroma_dst[1] = *(v_plane + (j * pitch_v) + k);
      chroma_dst += 2;
    }
    chroma_dst = chroma_tmp + va_image.pitches[1];
  }

  // Replicate the trailing row of pixels to fill-in any remaining rows
  // in the VA image. VA_API images heights have stricter alignment
  // requirement and may be larger than the actual image height, so
  // we need to replicate the image pixel rows to match the VME unit's
  // behavior to handle out-of-bound references.

  for (size_t j = height / 2; j < va_image.height / 2; j += 1) {
    uint8_t *chroma_tmp = chroma_dst;
    for (size_t k = 0; k < width / 2; k += 1) {
      chroma_dst[0] = *(u_plane + ((height / 2 - 1) * pitch_u) + k);
      chroma_dst[1] = *(v_plane + ((height / 2 - 1) * pitch_v) + k);
      chroma_dst += 2;
    }
    chroma_dst = chroma_tmp + va_image.pitches[1];
  }

  status = vamanager.vaUnmapBuffer(va_display, va_image.buf);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("write_va_surface failed");
  }

  status = vamanager.vaDestroyImage(va_display, va_image.image_id);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("write_va_surface failed");
  }
}

static void
run_vme_interop(const VmeInteropApplication::Arguments &args,
                compute::context &context, compute::command_queue &queue,
                compute::kernel &kernel, YuvCapture &capture,
                PlanarImage &planar_image, const VADisplay va_display,
                VASurfaceID &src_va_surface, VASurfaceID &ref_va_surface,
                compute::image2d &src_image, compute::image2d &ref_image,
                size_t frame_idx) {
  Timer timer;

  const size_t width = args.width;
  const size_t height = args.height;

  size_t mb_image_width = au::align_units(width, 16);
  size_t mb_image_height = au::align_units(height, 16);
  size_t mv_image_width = mb_image_width * 4;
  size_t mv_image_height = mb_image_height * 4;
  size_t mv_count = mv_image_width * mv_image_height;
  size_t mb_count = mb_image_width * mb_image_height;

  LOG_INFO << "Creating opencl mem objects...";

  au::PageAlignedVector<cl_short2> mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_short> residuals(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> shapes(au::align64(mb_count));

  cl_short2 default_predictor = {0, 0};
  au::PageAlignedVector<cl_short2> predictors(au::align64(mb_count),
                                              default_predictor);

  try {
    std::swap(ref_va_surface, src_va_surface);
    std::swap(ref_image, src_image);

    capture.get_sample(frame_idx, planar_image);
    timer.print("Read next YUV frame from disk to CPU linear memory.");

    write_va_surface(va_display, src_va_surface, planar_image);
    timer.print("Copied frame to GPU tiled memory using VAAPI.");

    compute::buffer mv_buffer(
        context, au::align64(mv_count * sizeof(cl_short2)),
        CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, mvs.data());
    compute::buffer residual_buffer(
        context, au::align64(mv_count * sizeof(cl_ushort)),
        CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, residuals.data());
    compute::buffer shape_buffer(
        context, au::align64(mb_count * sizeof(cl_uchar2)),
        CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, shapes.data());
    compute::buffer pred_buffer(
        context, au::align64(mb_count * sizeof(cl_short2)),
        CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, predictors.data());

    timer.print("Created opencl mem objects.");

    auto iterations = static_cast<cl_int>(mb_image_height);
    kernel.set_args(src_image, ref_image, pred_buffer, mv_buffer,
                    residual_buffer, shape_buffer, iterations);

    vamanager.acquire_va_surfaces(context.get_device().platform(), queue,
                                  src_image, ref_image);
    timer.print("Acquired VA surfaces");

    size_t local_size = 16;
    size_t global_size = au::align16(width);
    queue.enqueue_nd_range_kernel(kernel, 1, nullptr, &global_size,
                                  &local_size);
    timer.print("Kernel queued.");

    queue.finish();
    timer.print("Kernel finished.");

    vamanager.release_va_surfaces(context.get_device().platform(), queue,
                                  src_image, ref_image);
    timer.print("Released VA surfaces");

    planar_image.overlay_vectors(
        reinterpret_cast<motion_vector *>(mvs.data()),
        reinterpret_cast<inter_shape *>(shapes.data()));
  } catch (const std::exception &e) {
    LOG_FATAL << "Exception enountered in OpenCL host for vme_interop.";
    throw e;
  }
}

void VmeInteropApplication::run_os_specific_implementation(
    const Arguments &args, const compute::device &device) const {
  compute_samples::VAManager vamanager;
  int drm_fd = -1;

  VADisplay va_display = vamanager.get_va_display(drm_fd);

  if (vamanager.get_va_device(device.platform(), va_display) != device) {
    throw std::runtime_error("VA API interoperable device not found.");
  }

  cl_context_properties context_properties[] = {
      CL_CONTEXT_PLATFORM,
      reinterpret_cast<cl_context_properties>(device.platform().id()),
      CL_CONTEXT_VA_API_DISPLAY_INTEL,
      reinterpret_cast<cl_context_properties>(va_display), 0};

  compute::context context(device, context_properties);
  compute::command_queue queue(context, device);

  Timer timer;
  compute::program program = build_program(context, "vme_interop.cl");
  timer.print("Program created");

  compute::kernel kernel = program.create_kernel("vme_interop");
  timer.print("Kernel created");

  YuvCapture capture(args.input_yuv_path, args.width, args.height, args.frames);
  const size_t frame_count =
      (args.frames) != 0 ? args.frames : capture.get_num_frames();
  YuvWriter writer(args.width, args.height, frame_count, args.output_bmp);

  PlanarImage planar_image(args.width, args.height);
  capture.get_sample(0, planar_image);
  timer.print("Read YUV frame 0 from disk to CPU linear memory.");

  writer.append_frame(planar_image);

  VASurfaceID src_va_surface, ref_va_surface;
  vamanager.create_va_surface(args.width, args.height, va_display,
                              src_va_surface);
  vamanager.create_va_surface(args.width, args.height, va_display,
                              ref_va_surface);
  write_va_surface(va_display, src_va_surface, planar_image);
  timer.print("Copied frame 0 to GPU tiled memory using VAAPI.");
  compute::image2d_va src_image(context, &src_va_surface, 0);
  compute::image2d_va ref_image(context, &ref_va_surface, 0);

  for (size_t k = 1; k < frame_count; k++) {
    LOG_INFO << "Processing frame " << k << "...";
    run_vme_interop(args, context, queue, kernel, capture, planar_image,
                    va_display, src_va_surface, ref_va_surface, src_image,
                    ref_image, k);
    writer.append_frame(planar_image);
  }

  LOG_INFO << "Wrote " << frame_count << " frames with overlaid "
           << "motion vectors to " << args.output_yuv_path << " .";
  writer.write_to_file(args.output_yuv_path.c_str());

  if (drm_fd >= 0) {
    close(drm_fd);
  }
}
} // namespace compute_samples
