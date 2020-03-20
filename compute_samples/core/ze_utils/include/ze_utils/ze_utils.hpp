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

namespace compute_samples {
std::string to_string(const ze_api_version_t version);
std::string to_string(const ze_result_t result);
std::string to_string(const ze_command_queue_desc_version_t version);
std::string to_string(const ze_command_queue_flag_t flags);
std::string to_string(const ze_command_queue_mode_t mode);
std::string to_string(const ze_command_queue_priority_t priority);
std::string to_string(const ze_image_format_layout_t layout);
std::string to_string(const ze_image_format_type_t type);
std::string to_string(const ze_image_format_swizzle_t swizzle);
std::string to_string(const ze_image_flag_t flag);
std::string to_string(const ze_image_type_t type);
std::string to_string(const ze_image_desc_version_t version);
std::string to_string(const ze_device_type_t type);
std::string to_string(const ze_device_uuid_t uuid);
std::string to_string(const ze_memory_access_capabilities_t capabilities);
std::string to_string(const ze_driver_uuid_t uuid);
std::string to_string(const ze_native_kernel_uuid_t uuid);
std::string to_string(const ze_fp_capabilities_t capabilities);
} // namespace compute_samples

std::ostream &operator<<(std::ostream &os, const ze_api_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_result_t &x);
std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_desc_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_command_queue_flag_t &x);
std::ostream &operator<<(std::ostream &os, const ze_command_queue_mode_t &x);
std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_priority_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_desc_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_layout_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_type_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_swizzle_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_flag_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_type_t &x);
std::ostream &operator<<(std::ostream &os, const ze_device_type_t &x);
std::ostream &operator<<(std::ostream &os, const ze_device_uuid_t &x);
std::ostream &operator<<(std::ostream &os,
                         const ze_memory_access_capabilities_t &x);
std::ostream &operator<<(std::ostream &os, const ze_driver_uuid_t &x);
std::ostream &operator<<(std::ostream &os, const ze_native_kernel_uuid_t &x);
std::ostream &operator<<(std::ostream &os, const ze_fp_capabilities_t &x);

#endif
