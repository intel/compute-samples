/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_FP_TYPES_COMMON_HPP
#define COMPUTE_SAMPLES_FP_TYPES_COMMON_HPP

#include <cstdint>
#include <cstring>

#include "gtest/gtest.h"

namespace compute_samples {

static float uint32_to_float(uint32_t v) {
  float tmp;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  return tmp;
}

template <typename In, typename Type, typename Out> Out convert(const In a) {
  Type b(a);
  return static_cast<Out>(b);
}

template <typename T> struct PrintToHexStringParamName {
  template <class ParamType>
  std::string operator()(const testing::TestParamInfo<ParamType> &info) const {
    auto value = std::get<0>(info.param);
    std::stringstream ss;
    // std::showbase will not prefix 0
    // static_cast is needed for int8 types
    ss << "0x" << std::hex << std::setw(sizeof(T) * 2) << std::setfill('0')
       << std::noshowbase << static_cast<uint32_t>(value);
    return ss.str();
  }
};

template <> struct PrintToHexStringParamName<float> {
  template <class ParamType>
  std::string operator()(const testing::TestParamInfo<ParamType> &info) const {
    uint32_t tmp;
    auto value = std::get<0>(info.param);
    std::memcpy(&tmp, &value, sizeof(uint32_t));
    std::stringstream ss;
    // std::showbase will not prefix 0
    ss << "0x" << std::hex << std::setw(sizeof(tmp) * 2) << std::setfill('0')
       << std::noshowbase << tmp;
    return ss.str();
  }
};

} // namespace compute_samples

#endif