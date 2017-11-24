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
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#ifndef COMPUTE_SAMPLES_ALIGN_UTILS_HPP
#define COMPUTE_SAMPLES_ALIGN_UTILS_HPP

#include <boost/align/aligned_allocator.hpp>
#include <cstdint>

namespace compute_samples {

namespace align_utils {
template <typename T>
using PageAlignedAllocator = boost::alignment::aligned_allocator<T, 4096>;

template <typename T>
using PageAlignedVector =
    std::vector<T, boost::alignment::aligned_allocator<T, 4096>>;

static uint32_t align(uint32_t value, uint32_t alignment) {
  return ((value + alignment - 1) / alignment) * alignment;
}

static uint32_t align16(uint32_t value) { return align(value, 16); }

static uint32_t align32(uint32_t value) { return align(value, 32); }

static uint32_t align64(uint32_t value) { return align(value, 64); }

static uint32_t align_units(uint32_t value, uint32_t alignment) {
  return align(value, alignment) / alignment;
}
};
}

#endif
