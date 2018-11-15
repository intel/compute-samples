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
#include "vme_interop/vme_interop_linux.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <CL/cl_ext_intel.h>

#include "align_utils/align_utils.hpp"
namespace au = compute_samples::align_utils;

#include "boost/compute/intel/image/image2d_va.hpp"
#include "timer/timer.hpp"
#include "yuv_utils/yuv_utils.hpp"
#include "ocl_utils/ocl_utils.hpp"

#include <CL/cl_va_api_media_sharing_intel.h>
#include <fcntl.h>

#ifndef DRM_RENDER_NODE_PATH
#define DRM_RENDER_NODE_PATH "/dev/dri/renderD128"
#endif

#ifndef DRM_DEVICE_PATH
#define DRM_DEVICE_PATH "/dev/dri/card0"
#endif

namespace compute_samples {

VAManager::VAManager() {
  src::logger logger;
  libVaHandle = dlopen("libva.so", RTLD_LAZY);
  libVaX11Handle = dlopen("libva-x11.so", RTLD_LAZY);
  libVaDRMHandle = dlopen("libva-drm.so", RTLD_LAZY);

  if (libVaHandle == nullptr) {
    throw std::runtime_error("Loading libva.so failed");
  }
  if (libVaDRMHandle == nullptr) {
    throw std::runtime_error("Loading libva-drm.so failed");
  }
  if (libVaX11Handle == nullptr) {
    throw std::runtime_error("Loading libvax11.so failed");
  }

  vaGetDisplay = (vaGetDisplayFPTR)dlsym(libVaX11Handle, "vaGetDisplay");
  if (!vaGetDisplay) {
    BOOST_LOG(logger) << "dlsym error vaGetDisplay: " << dlerror();
  }

  XOpenDisplay = (XOpenDisplayFPTR)dlsym(libVaX11Handle, "XOpenDisplay");
  if (!XOpenDisplay) {
    BOOST_LOG(logger) << "dlsym error XOpenDisplay: " << dlerror();
  }

  vaGetDisplayDRM =
      (vaGetDisplayDRMFPTR)dlsym(libVaDRMHandle, "vaGetDisplayDRM");
  if (!vaGetDisplayDRM) {
    BOOST_LOG(logger) << "dlsym error vaGetDisplayDRM: " << dlerror();
  }

  vaInitialize = (vaInitializeFPTR)dlsym(libVaHandle, "vaInitialize");
  vaDeriveImage = (vaDeriveImageFPTR)dlsym(libVaHandle, "vaDeriveImage");
  vaMapBuffer = (vaMapBufferFPTR)dlsym(libVaHandle, "vaMapBuffer");
  vaUnmapBuffer = (vaUnmapBufferFPTR)dlsym(libVaHandle, "vaUnmapBuffer");
  vaDestroyImage = (vaDestroyImageFPTR)dlsym(libVaHandle, "vaDestroyImage");
  vaCreateSurfaces =
      (vaCreateSurfacesFPTR)dlsym(libVaHandle, "vaCreateSurfaces");
}

VAManager::~VAManager() {
  dlclose(libVaHandle);
  dlclose(libVaX11Handle);
  dlclose(libVaDRMHandle);
}

VAManager manager;

static VADisplay get_va_display() {
  VADisplay va_display;
  int major_version, minor_version;
  VAStatus status = -1;

  VADisplay display = manager.XOpenDisplay(nullptr);
  if (display) {
    va_display = manager.vaGetDisplay(display);
    status = manager.vaInitialize(va_display, &major_version, &minor_version);
  }

  if (status != VA_STATUS_SUCCESS) {
    src::logger logger;
    BOOST_LOG(logger) << "INFO: initializing VADisplay from X11 display "
                         "failed.  Trying render node";
    int drm_fd = open(DRM_RENDER_NODE_PATH, O_RDWR);
    if (drm_fd < 0) {
      BOOST_LOG(logger) << "INFO: initializing VADisplay from render node "
                           "failed. Trying card0 handle";
      drm_fd = open(DRM_DEVICE_PATH, O_RDWR);
      if (drm_fd < 0) {
        BOOST_LOG(logger) << "ERROR: initializing VADisplay from card0 failed.";
        status = VA_STATUS_ERROR_OPERATION_FAILED;
      }
    }
    va_display = manager.vaGetDisplayDRM(drm_fd);
    status = manager.vaInitialize(va_display, &major_version, &minor_version);
  }

  if ((va_display == NULL) || (status != VA_STATUS_SUCCESS)) {
    throw std::runtime_error("ERROR: get_va_display failed\n");
  }

  return va_display;
}

compute::device get_va_device(const compute::platform &platform,
                              const VADisplay va_display) {
  clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn cl_get_va_device_ids =
      (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
          platform.get_extension_function_address(
              "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
  if (!cl_get_va_device_ids) {
    throw std::runtime_error("clGetExtensionFunctionAddressForPlatform("
                             "clGetDeviceIDsFromVA_APIMediaAdapterINTEL) "
                             "returned NULL.");
  }

  cl_int error;
  cl_device_id va_devices[1] = {0};
  cl_uint num_va_devices = 0;
  error = cl_get_va_device_ids(platform.id(), CL_VA_API_DISPLAY_INTEL,
                               va_display, CL_ALL_DEVICES_FOR_VA_API_INTEL, 0,
                               NULL, &num_va_devices);

  error = cl_get_va_device_ids(
      platform.id(), CL_VA_API_DISPLAY_INTEL, va_display,
      CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 1, va_devices, &num_va_devices);

  if (error != CL_SUCCESS && error != CL_DEVICE_NOT_FOUND) {
    throw std::runtime_error("ERROR: get_va_device failed");
  }

  return compute::device(va_devices[0]);
}

static void create_va_surface(uint32_t width, uint32_t height,
                              const VADisplay va_display,
                              VASurfaceID &va_surface) {
  if (manager.vaCreateSurfaces(va_display, VA_FOURCC_NV12, width, height,
                               &va_surface, 1, NULL, 0) != VA_STATUS_SUCCESS) {
    throw std::runtime_error("vaCreateSurfaces() failed!\n");
  }
}

static void acquire_va_surfaces(const compute::platform &platform,
                                const compute::command_queue &queue,
                                compute::image2d &src_image,
                                compute::image2d &ref_image) {
  cl_mem images_shared[] = {src_image.get(), ref_image.get()};
  clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn cl_acquire_va_surface =
      (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
          platform.get_extension_function_address(
              "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
  if (!cl_acquire_va_surface) {
    throw std::runtime_error("ERROR: acquire_va_surface failed");
  }
  cl_acquire_va_surface(queue.get(), 2, images_shared, 0, NULL, NULL);
}

static void release_va_surfaces(const compute::platform &platform,
                                const compute::command_queue &queue,
                                compute::image2d &src_image,
                                compute::image2d &ref_image) {
  cl_mem images_shared[] = {src_image.get(), ref_image.get()};
  clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn cl_release_va_surface =
      (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
          platform.get_extension_function_address(
              "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
  if (!cl_release_va_surface) {
    throw std::runtime_error("ERROR: release_va_surface failed");
  }
  cl_release_va_surface(queue.get(), 2, images_shared, 0, NULL, NULL);
}

static void write_va_surface(const VADisplay va_display,
                             const VASurfaceID va_surface,
                             const PlanarImage &planar_image) {
  VAImage va_image;
  VAStatus status = manager.vaDeriveImage(va_display, va_surface, &va_image);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("ERROR: write_va_surface failed");
  }

  uint8_t *ptr;
  status = manager.vaMapBuffer(va_display, va_image.buf, (void **)&ptr);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("ERROR: write_va_surface failed");
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

  status = manager.vaUnmapBuffer(va_display, va_image.buf);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("ERROR: write_va_surface failed");
  }

  status = manager.vaDestroyImage(va_display, va_image.image_id);
  if (status != VA_STATUS_SUCCESS) {
    throw std::runtime_error("ERROR: write_va_surface failed");
  }
}

static void
run_vme_interop(const VmeInteropApplication::Arguments &args,
                compute::context &context, compute::command_queue &queue,
                compute::kernel &kernel, Capture &capture,
                PlanarImage &planar_image, const VADisplay va_display,
                VASurfaceID &src_va_surface, VASurfaceID &ref_va_surface,
                compute::image2d &src_image, compute::image2d &ref_image,
                size_t frame_idx) {
  src::logger logger;
  Timer timer(logger);

  const size_t width = args.width;
  const size_t height = args.height;

  size_t mb_image_width = au::align_units(width, 16);
  size_t mb_image_height = au::align_units(height, 16);
  size_t mv_image_width = mb_image_width * 4;
  size_t mv_image_height = mb_image_height * 4;
  size_t mv_count = mv_image_width * mv_image_height;
  size_t mb_count = mb_image_width * mb_image_height;

  BOOST_LOG(logger) << "Creating opencl mem objects...";

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

    cl_int iterations = static_cast<cl_int>(mb_image_height);
    kernel.set_args(src_image, ref_image, pred_buffer, mv_buffer,
                    residual_buffer, shape_buffer, iterations);

    acquire_va_surfaces(context.get_device().platform(), queue, src_image,
                        ref_image);
    timer.print("Acquired VA surfaces");

    size_t local_size = 16;
    size_t global_size = au::align16(width);
    queue.enqueue_nd_range_kernel(kernel, 1, nullptr, &global_size,
                                  &local_size);
    timer.print("Kernel queued.");

    queue.finish();
    timer.print("Kernel finished.");

    release_va_surfaces(context.get_device().platform(), queue, src_image,
                        ref_image);
    timer.print("Released VA surfaces");

    planar_image.overlay_vectors(
        reinterpret_cast<motion_vector *>(mvs.data()),
        reinterpret_cast<inter_shape *>(shapes.data()));
  } catch (const std::exception &e) {
    BOOST_LOG(logger) << "Exception enountered in OpenCL host for vme_interop.";
    throw e;
  }
}

void VmeInteropApplication::run_os_specific_implementation(
    std::vector<std::string> &command_line, const Arguments &args,
    const compute::device &device) const {
  VADisplay va_display = get_va_display();

  if (get_va_device(device.platform(), va_display) != device) {
    throw std::runtime_error("ERROR: VA API interoperable device not found.");
  }

  cl_context_properties context_properties[] = {
      CL_CONTEXT_PLATFORM, (cl_context_properties)(device.platform().id()),
      CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties)(va_display), 0};

  compute::context context(device, context_properties);
  compute::command_queue queue(context, device);

  src::logger logger;
  Timer timer(logger);
  compute::program program = build_program(context, "vme_interop.cl");
  timer.print("Program created");

  compute::kernel kernel = program.create_kernel("vme_interop");
  timer.print("Kernel created");

  Capture *capture = Capture::create_file_capture(
      args.input_yuv_path, args.width, args.height, args.frames);
  const size_t frame_count =
      (args.frames) ? args.frames : capture->get_num_frames();
  FrameWriter *writer = FrameWriter::create_frame_writer(
      args.width, args.height, frame_count, args.output_bmp);

  PlanarImage *planar_image =
      PlanarImage::create_planar_image(args.width, args.height);
  capture->get_sample(0, *planar_image);
  timer.print("Read YUV frame 0 from disk to CPU linear memory.");

  writer->append_frame(*planar_image);

  VASurfaceID src_va_surface, ref_va_surface;
  create_va_surface(args.width, args.height, va_display, src_va_surface);
  create_va_surface(args.width, args.height, va_display, ref_va_surface);
  write_va_surface(va_display, src_va_surface, *planar_image);
  timer.print("Copied frame 0 to GPU tiled memory using VAAPI.");
  compute::image2d_va src_image(context, &src_va_surface, -1);
  compute::image2d_va ref_image(context, &ref_va_surface, -1);

  for (size_t k = 1; k < frame_count; k++) {
    BOOST_LOG(logger) << "Processing frame " << k << "...\n";
    run_vme_interop(args, context, queue, kernel, *capture, *planar_image,
                    va_display, src_va_surface, ref_va_surface, src_image,
                    ref_image, k);
    writer->append_frame(*planar_image);
  }

  BOOST_LOG(logger) << "Wrote " << frame_count << " frames with overlaid "
                    << "motion vectors to " << args.output_yuv_path << " .\n";
  writer->write_to_file(args.output_yuv_path.c_str());

  FrameWriter::release(writer);
  Capture::release(capture);
  PlanarImage::release_image(planar_image);
}
} // namespace compute_samples
