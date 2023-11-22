/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "va_utils/va_utils.hpp"
#include "logging/logging.hpp"
#include <CL/cl_va_api_media_sharing_intel.h>
#include <fcntl.h>

#ifndef DRM_RENDER_NODE_PATH
#define DRM_RENDER_NODE_PATH "/dev/dri/renderD128"
#endif

#ifndef DRM_DEVICE_PATH
#define DRM_DEVICE_PATH "/dev/dri/card0"
#endif

namespace compute = boost::compute;

namespace compute_samples {

VAManager::VAManager() {
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

  char *error = nullptr;

  vaGetDisplay =
      reinterpret_cast<vaGetDisplayFPTR>(dlsym(libVaX11Handle, "vaGetDisplay"));
  error = dlerror();
  if ((vaGetDisplay == nullptr) && error != nullptr) {
    LOG_ERROR << "dlsym error vaGetDisplay: " << error;
  }

  XOpenDisplay =
      reinterpret_cast<XOpenDisplayFPTR>(dlsym(libVaX11Handle, "XOpenDisplay"));
  error = dlerror();
  if ((XOpenDisplay == nullptr) && error != nullptr) {
    LOG_ERROR << "dlsym error XOpenDisplay: " << error;
  }

  vaGetDisplayDRM = reinterpret_cast<vaGetDisplayDRMFPTR>(
      dlsym(libVaDRMHandle, "vaGetDisplayDRM"));
  error = dlerror();
  if ((vaGetDisplayDRM == nullptr) && error != nullptr) {
    LOG_ERROR << "dlsym error vaGetDisplayDRM: " << error;
  }

  vaInitialize =
      reinterpret_cast<vaInitializeFPTR>(dlsym(libVaHandle, "vaInitialize"));
  vaDeriveImage =
      reinterpret_cast<vaDeriveImageFPTR>(dlsym(libVaHandle, "vaDeriveImage"));
  vaMapBuffer =
      reinterpret_cast<vaMapBufferFPTR>(dlsym(libVaHandle, "vaMapBuffer"));
  vaUnmapBuffer =
      reinterpret_cast<vaUnmapBufferFPTR>(dlsym(libVaHandle, "vaUnmapBuffer"));
  vaDestroyImage = reinterpret_cast<vaDestroyImageFPTR>(
      dlsym(libVaHandle, "vaDestroyImage"));
  vaCreateSurfaces = reinterpret_cast<vaCreateSurfacesFPTR>(
      dlsym(libVaHandle, "vaCreateSurfaces"));
  vaQueryImageFormats = reinterpret_cast<vaQueryImageFormatsPFN>(
      dlsym(libVaHandle, "vaQueryImageFormats"));
  vaMaxNumImageFormats = reinterpret_cast<vaMaxNumImageFormatsPFN>(
      dlsym(libVaHandle, "vaMaxNumImageFormats"));
}

VAManager::~VAManager() {
  dlclose(libVaHandle);
  dlclose(libVaX11Handle);
  dlclose(libVaDRMHandle);
  if (drm_fd >= 0) {
    close(drm_fd);
  }
}

VADisplay VAManager::get_va_display() {
  VADisplay va_display = nullptr;
  int major_version, minor_version;
  VAStatus status = -1;

  VADisplay display = XOpenDisplay(nullptr);
  if (display != nullptr) {
    va_display = vaGetDisplay(display);
    status = vaInitialize(va_display, &major_version, &minor_version);
  }

  if (status != VA_STATUS_SUCCESS) {
    LOG_INFO << "initializing VADisplay from X11 display "
                "failed.  Trying render node";
    if (drm_fd < 0) {
      drm_fd = open(DRM_RENDER_NODE_PATH, O_RDWR);
      if (drm_fd < 0) {
        LOG_INFO << "initializing VADisplay from render node "
                    "failed. Trying card0 handle";
        drm_fd = open(DRM_DEVICE_PATH, O_RDWR);
        if (drm_fd < 0) {
          LOG_ERROR << "initializing VADisplay from card0 failed.";
        }
      }
      va_display = vaGetDisplayDRM(drm_fd);
      status = vaInitialize(va_display, &major_version, &minor_version);
    }
  }

  if ((va_display == nullptr) || (status != VA_STATUS_SUCCESS)) {
    throw std::runtime_error("get_va_display failed");
  }

  return va_display;
}

compute::device VAManager::get_va_device(const compute::platform &platform,
                                         const VADisplay va_display) {
  auto cl_get_va_device_ids =
      reinterpret_cast<clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn>(
          platform.get_extension_function_address(
              "clGetDeviceIDsFromVA_APIMediaAdapterINTEL"));
  if (cl_get_va_device_ids == nullptr) {
    throw std::runtime_error("clGetExtensionFunctionAddressForPlatform("
                             "clGetDeviceIDsFromVA_APIMediaAdapterINTEL) "
                             "returned nullptr.");
  }

  cl_int error;
  cl_uint num_va_devices = 0;
  error = cl_get_va_device_ids(platform.id(), CL_VA_API_DISPLAY_INTEL,
                               va_display, CL_ALL_DEVICES_FOR_VA_API_INTEL, 0,
                               nullptr, &num_va_devices);

  cl_device_id va_device;
  error = cl_get_va_device_ids(
      platform.id(), CL_VA_API_DISPLAY_INTEL, va_display,
      CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 1, &va_device, nullptr);

  if (error != CL_SUCCESS && error != CL_DEVICE_NOT_FOUND) {
    throw std::runtime_error("get_va_device failed");
  }

  return compute::device(va_device);
}

void VAManager::create_va_surface(uint32_t width, uint32_t height,
                                  const VADisplay va_display,
                                  VASurfaceID &va_surface) {
  if (vaCreateSurfaces(va_display, VA_FOURCC_NV12, width, height, &va_surface,
                       1, nullptr, 0) != VA_STATUS_SUCCESS) {
    throw std::runtime_error("vaCreateSurfaces() failed!");
  }
}

void VAManager::acquire_va_surfaces(const compute::platform &platform,
                                    const compute::command_queue &queue,
                                    compute::image2d &src_image,
                                    compute::image2d &ref_image) {
  cl_mem images_shared[] = {src_image.get(), ref_image.get()};
  auto cl_acquire_va_surface =
      reinterpret_cast<clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn>(
          platform.get_extension_function_address(
              "clEnqueueAcquireVA_APIMediaSurfacesINTEL"));
  if (cl_acquire_va_surface == nullptr) {
    throw std::runtime_error("acquire_va_surface failed");
  }
  cl_acquire_va_surface(queue.get(), 2, images_shared, 0, nullptr, nullptr);
}

void VAManager::release_va_surfaces(const compute::platform &platform,
                                    const compute::command_queue &queue,
                                    compute::image2d &src_image,
                                    compute::image2d &ref_image) {
  cl_mem images_shared[] = {src_image.get(), ref_image.get()};
  auto cl_release_va_surface =
      reinterpret_cast<clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn>(
          platform.get_extension_function_address(
              "clEnqueueReleaseVA_APIMediaSurfacesINTEL"));
  if (cl_release_va_surface == nullptr) {
    throw std::runtime_error("release_va_surface failed");
  }
  cl_release_va_surface(queue.get(), 2, images_shared, 0, nullptr, nullptr);
}

} // namespace compute_samples
