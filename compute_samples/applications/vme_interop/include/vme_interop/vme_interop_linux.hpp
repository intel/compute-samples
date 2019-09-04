/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_INTEROP_LINUX_HPP
#define COMPUTE_SAMPLES_VME_INTEROP_LINUX_HPP

#include <string>
#include <vector>
#include <va/va.h>
#include <dlfcn.h>
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

  ~VAManager();

private:
  void *libVaHandle;
  void *libVaX11Handle;
  void *libVaDRMHandle;
};
} // namespace compute_samples
#endif
