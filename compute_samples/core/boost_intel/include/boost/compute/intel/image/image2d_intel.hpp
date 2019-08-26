/*
 * Copyright(c) 2019 Intel Corporation
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

#ifndef BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP
#define BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP

#include <boost/compute/image/image_object.hpp>

namespace boost {
namespace compute {

class image2d_intel : public image_object {
  using image_object::image_object;

public:
  image2d_intel() : image_object() {}

  image2d_intel(const context &context, const cl_image_desc &desc,
                const cl_image_format &format, cl_mem_flags flags = read_write,
                void *host_ptr = 0)
      : image_object(create_image(context, flags, format, desc, host_ptr),
                     false) {}

private:
  static cl_mem create_image(const context &context, cl_mem_flags flags,
                             const cl_image_format &format,
                             const cl_image_desc &desc, void *host_ptr) {
    cl_int error = CL_SUCCESS;
    cl_mem m = clCreateImage(context, flags, &format, &desc, host_ptr, &error);
    if (!m) {
      BOOST_THROW_EXCEPTION(opencl_error(error));
    }
    return m;
  }
};

namespace detail {

template <>
struct set_kernel_arg<image2d_intel> : public set_kernel_arg<image_object> {};

} // namespace detail
} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP
