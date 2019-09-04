/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
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

inline uint32_t align(uint32_t value, uint32_t alignment) {
  return ((value + alignment - 1) / alignment) * alignment;
}

inline uint32_t align16(uint32_t value) { return align(value, 16); }

inline uint32_t align32(uint32_t value) { return align(value, 32); }

inline uint32_t align64(uint32_t value) { return align(value, 64); }

inline uint32_t align_units(uint32_t value, uint32_t alignment) {
  return align(value, alignment) / alignment;
}
}; // namespace align_utils
} // namespace compute_samples

#endif
