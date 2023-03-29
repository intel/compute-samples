/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_UTILS_HPP
#define COMPUTE_SAMPLES_ZE_UTILS_HPP

#include <vector>
#include <string>

#include "ze_api.h"
#include "zet_api.h"
#include "utils/utils.hpp"

namespace compute_samples {
std::string to_string(ze_result_t result);
std::string to_string(ze_device_memory_property_flag_t flag);
std::string to_string(ze_ipc_property_flag_t flag);
std::string to_string(ze_device_type_t type);
std::string to_string(ze_memory_access_cap_flag_t flag);
std::string to_string(ze_external_memory_type_flag_t flag);
std::string to_string(ze_device_cache_property_flag_t flag);
std::string to_string(ze_device_property_flag_t flag);
std::string to_string(ze_device_module_flag_t flag);
std::string to_string(ze_device_fp_flag_t flag);
std::string to_string(ze_scheduling_hint_exp_flag_t flag);
std::string to_string(ze_device_fp_atomic_ext_flag_t flag);
std::string to_string(ze_device_raytracing_ext_flag_t flag);
std::string to_string(ze_command_queue_group_property_flag_t flag);
std::string to_string(zet_device_debug_property_flag_t flag);

template <typename T> std::string flags_to_string(uint32_t flags) {
  const size_t bits = 8;
  std::vector<std::string> output;
  for (size_t i = 0; i < sizeof(flags) * bits; ++i) {
    const size_t mask = 1UL << i;
    const auto flag = flags & mask;
    if (flag != 0) {
      output.emplace_back(to_string(static_cast<T>(flag)));
    }
  }
  std::sort(output.begin(), output.end());
  return join_strings(output, " | ");
}

void throw_if_failed(ze_result_t result, const std::string &function_name);
} // namespace compute_samples

#endif
