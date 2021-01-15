/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VA_UTILS_HPP
#define COMPUTE_SAMPLES_VA_UTILS_HPP

#include <va/va.h>
#include <dlfcn.h>
#include <boost/compute/core.hpp>

namespace compute_samples {

typedef VAStatus (*vaInitializeFPTR)(VADisplay, int *, int *);
typedef VADisplay (*vaGetDisplayFPTR)(VADisplay);
typedef VADisplay (*XOpenDisplayFPTR)(char *);
typedef VADisplay (*vaGetDisplayDRMFPTR)(int);
typedef VAStatus (*vaDeriveImageFPTR)(VADisplay, VASurfaceID, VAImage *);
typedef VAStatus (*vaMapBufferFPTR)(VADisplay, VABufferID, void **);
typedef VAStatus (*vaUnmapBufferFPTR)(VADisplay, VABufferID);
typedef VAStatus (*vaDestroyImageFPTR)(VADisplay, VAImageID);
typedef VAStatus (*vaCreateSurfacesFPTR)(VADisplay, unsigned int, unsigned int,
                                         unsigned int, VASurfaceID *,
                                         unsigned int, VASurfaceAttrib *,
                                         unsigned int);
typedef VAStatus (*vaQueryImageFormatsPFN)(VADisplay, VAImageFormat *, int *);
typedef int (*vaMaxNumImageFormatsPFN)(VADisplay);

class VAManager {
public:
  VAManager();
  vaInitializeFPTR vaInitialize;
  vaGetDisplayFPTR vaGetDisplay;
  XOpenDisplayFPTR XOpenDisplay;
  vaGetDisplayDRMFPTR vaGetDisplayDRM;
  vaDeriveImageFPTR vaDeriveImage;
  vaMapBufferFPTR vaMapBuffer;
  vaUnmapBufferFPTR vaUnmapBuffer;
  vaDestroyImageFPTR vaDestroyImage;
  vaCreateSurfacesFPTR vaCreateSurfaces;
  vaQueryImageFormatsPFN vaQueryImageFormats;
  vaMaxNumImageFormatsPFN vaMaxNumImageFormats;
  VADisplay get_va_display(int &);
  boost::compute::device get_va_device(const boost::compute::platform &,
                                       const VADisplay);

  void create_va_surface(uint32_t, uint32_t, const VADisplay, VASurfaceID &);

  void acquire_va_surfaces(const boost::compute::platform &,
                           const boost::compute::command_queue &,
                           boost::compute::image2d &,
                           boost::compute::image2d &);

  void release_va_surfaces(const boost::compute::platform &,
                           const boost::compute::command_queue &,
                           boost::compute::image2d &,
                           boost::compute::image2d &);

  ~VAManager();

private:
  void *libVaHandle;
  void *libVaX11Handle;
  void *libVaDRMHandle;
};
} // namespace compute_samples
#endif
