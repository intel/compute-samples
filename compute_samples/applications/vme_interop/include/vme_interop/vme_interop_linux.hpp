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
