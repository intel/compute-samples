/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_utils/ze_utils.hpp"
#include "logging/logging.hpp"

#include <iomanip>

namespace compute_samples {
std::string to_string(const ze_api_version_t version) {
  std::stringstream ss;
  ss << ZE_MAJOR_VERSION(version) << "." << ZE_MINOR_VERSION(version);
  return ss.str();
}

std::string to_string(const ze_result_t result) {
  if (result == ZE_RESULT_SUCCESS) {
    return "ZE_RESULT_SUCCESS";
  }
  if (result == ZE_RESULT_NOT_READY) {
    return "ZE_RESULT_NOT_READY";
  }
  if (result == ZE_RESULT_ERROR_DEVICE_LOST) {
    return "ZE_RESULT_ERROR_DEVICE_LOST";
  }
  if (result == ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY) {
    return "ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY";
  }
  if (result == ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY) {
    return "ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY";
  }
  if (result == ZE_RESULT_ERROR_MODULE_BUILD_FAILURE) {
    return "ZE_RESULT_ERROR_MODULE_BUILD_FAILURE";
  }
  if (result == ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS) {
    return "ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS";
  }
  if (result == ZE_RESULT_ERROR_NOT_AVAILABLE) {
    return "ZE_RESULT_ERROR_NOT_AVAILABLE";
  }
  if (result == ZE_RESULT_ERROR_UNINITIALIZED) {
    return "ZE_RESULT_ERROR_UNINITIALIZED";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_VERSION) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_VERSION";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_FEATURE) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_FEATURE";
  }
  if (result == ZE_RESULT_ERROR_INVALID_ARGUMENT) {
    return "ZE_RESULT_ERROR_INVALID_ARGUMENT";
  }
  if (result == ZE_RESULT_ERROR_INVALID_NULL_HANDLE) {
    return "ZE_RESULT_ERROR_INVALID_NULL_HANDLE";
  }
  if (result == ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE) {
    return "ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE";
  }
  if (result == ZE_RESULT_ERROR_INVALID_NULL_POINTER) {
    return "ZE_RESULT_ERROR_INVALID_NULL_POINTER";
  }
  if (result == ZE_RESULT_ERROR_INVALID_SIZE) {
    return "ZE_RESULT_ERROR_INVALID_SIZE";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_SIZE) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_SIZE";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT";
  }
  if (result == ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT) {
    return "ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT";
  }
  if (result == ZE_RESULT_ERROR_INVALID_ENUMERATION) {
    return "ZE_RESULT_ERROR_INVALID_ENUMERATION";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION";
  }
  if (result == ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT) {
    return "ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT";
  }
  if (result == ZE_RESULT_ERROR_INVALID_NATIVE_BINARY) {
    return "ZE_RESULT_ERROR_INVALID_NATIVE_BINARY";
  }
  if (result == ZE_RESULT_ERROR_INVALID_GLOBAL_NAME) {
    return "ZE_RESULT_ERROR_INVALID_GLOBAL_NAME";
  }
  if (result == ZE_RESULT_ERROR_INVALID_KERNEL_NAME) {
    return "ZE_RESULT_ERROR_INVALID_KERNEL_NAME";
  }
  if (result == ZE_RESULT_ERROR_INVALID_FUNCTION_NAME) {
    return "ZE_RESULT_ERROR_INVALID_FUNCTION_NAME";
  }
  if (result == ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION) {
    return "ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION";
  }
  if (result == ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION) {
    return "ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION";
  }
  if (result == ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX) {
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX";
  }
  if (result == ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE) {
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE";
  }
  if (result == ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE) {
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE";
  }
  if (result == ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE) {
    return "ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE";
  }
  if (result == ZE_RESULT_ERROR_OVERLAPPING_REGIONS) {
    return "ZE_RESULT_ERROR_OVERLAPPING_REGIONS";
  }
  if (result == ZE_RESULT_ERROR_UNKNOWN) {
    return "ZE_RESULT_ERROR_UNKNOWN";
  }
  return "Unknown ze_result_t value: " +
         std::to_string(static_cast<int>(result));
}

std::string to_string(const ze_command_queue_desc_version_t version) {
  if (version == ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT) {
    return "ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT";
  }
  return "Unknown ze_command_queue_desc_version_t value: " +
         std::to_string(static_cast<int>(version));
}

std::string to_string(const ze_command_queue_flag_t flags) {
  if (flags == ZE_COMMAND_QUEUE_FLAG_NONE) {
    return "ZE_COMMAND_QUEUE_FLAG_NONE";
  }
  if (flags == ZE_COMMAND_QUEUE_FLAG_COPY_ONLY) {
    return "ZE_COMMAND_QUEUE_FLAG_COPY_ONLY";
  }
  if (flags == ZE_COMMAND_QUEUE_FLAG_LOGICAL_ONLY) {
    return "ZE_COMMAND_QUEUE_FLAG_LOGICAL_ONLY";
  }
  if (flags == ZE_COMMAND_QUEUE_FLAG_SINGLE_SLICE_ONLY) {
    return "ZE_COMMAND_QUEUE_FLAG_SINGLE_SLICE_ONLY";
  }
  return "Unknown ze_command_queue_flag_t value: " +
         std::to_string(static_cast<int>(flags));
}

std::string to_string(const ze_command_queue_mode_t mode) {
  if (mode == ZE_COMMAND_QUEUE_MODE_DEFAULT) {
    return "ZE_COMMAND_QUEUE_MODE_DEFAULT";
  }
  if (mode == ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS) {
    return "ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS";
  }
  if (mode == ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS) {
    return "ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS";
  }
  return "Unknown ze_command_queue_mode_t value: " +
         std::to_string(static_cast<int>(mode));
}

std::string to_string(const ze_command_queue_priority_t priority) {
  if (priority == ZE_COMMAND_QUEUE_PRIORITY_NORMAL) {
    return "ZE_COMMAND_QUEUE_PRIORITY_NORMAL";
  }
  if (priority == ZE_COMMAND_QUEUE_PRIORITY_LOW) {
    return "ZE_COMMAND_QUEUE_PRIORITY_LOW";
  }
  if (priority == ZE_COMMAND_QUEUE_PRIORITY_HIGH) {
    return "ZE_COMMAND_QUEUE_PRIORITY_HIGH";
  }
  return "Unknown ze_command_queue_priority_t value: " +
         std::to_string(static_cast<int>(priority));
}

std::string to_string(const ze_image_desc_version_t version) {
  if (version == ZE_IMAGE_DESC_VERSION_CURRENT) {
    return "ZE_IMAGE_DESC_VERSION_CURRENT";
  }
  return "Unknown ze_image_desc_version_t value: " +
         std::to_string(static_cast<int>(version));
}

std::string to_string(const ze_image_format_layout_t layout) {
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_8) {
    return "ZE_IMAGE_FORMAT_LAYOUT_8";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_16) {
    return "ZE_IMAGE_FORMAT_LAYOUT_16";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_32) {
    return "ZE_IMAGE_FORMAT_LAYOUT_32";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_8_8) {
    return "ZE_IMAGE_FORMAT_LAYOUT_8_8";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8) {
    return "ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_16_16) {
    return "ZE_IMAGE_FORMAT_LAYOUT_16_16";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16) {
    return "ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_32_32) {
    return "ZE_IMAGE_FORMAT_LAYOUT_32_32";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32) {
    return "ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2) {
    return "ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_11_11_10) {
    return "ZE_IMAGE_FORMAT_LAYOUT_11_11_10";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_5_6_5) {
    return "ZE_IMAGE_FORMAT_LAYOUT_5_6_5";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1) {
    return "ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_4_4_4_4) {
    return "ZE_IMAGE_FORMAT_LAYOUT_4_4_4_4";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_Y8) {
    return "ZE_IMAGE_FORMAT_LAYOUT_Y8";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_NV12) {
    return "ZE_IMAGE_FORMAT_LAYOUT_NV12";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_YUYV) {
    return "ZE_IMAGE_FORMAT_LAYOUT_YUYV";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_VYUY) {
    return "ZE_IMAGE_FORMAT_LAYOUT_VYUY";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_YVYU) {
    return "ZE_IMAGE_FORMAT_LAYOUT_YVYU";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_UYVY) {
    return "ZE_IMAGE_FORMAT_LAYOUT_UYVY";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_AYUV) {
    return "ZE_IMAGE_FORMAT_LAYOUT_AYUV";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_YUAV) {
    return "ZE_IMAGE_FORMAT_LAYOUT_YUAV";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_P010) {
    return "ZE_IMAGE_FORMAT_LAYOUT_P010";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_Y410) {
    return "ZE_IMAGE_FORMAT_LAYOUT_Y410";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_P012) {
    return "ZE_IMAGE_FORMAT_LAYOUT_P012";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_Y16) {
    return "ZE_IMAGE_FORMAT_LAYOUT_Y16";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_P016) {
    return "ZE_IMAGE_FORMAT_LAYOUT_P016";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_Y216) {
    return "ZE_IMAGE_FORMAT_LAYOUT_Y216";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_P216) {
    return "ZE_IMAGE_FORMAT_LAYOUT_P216";
  }
  if (layout == ZE_IMAGE_FORMAT_LAYOUT_P416) {
    return "ZE_IMAGE_FORMAT_LAYOUT_P416";
  }
  return "Unknown ze_image_format_layout_t value: " +
         std::to_string(static_cast<int>(layout));
}

std::string to_string(const ze_image_format_type_t type) {
  if (type == ZE_IMAGE_FORMAT_TYPE_UINT) {
    return "ZE_IMAGE_FORMAT_TYPE_UINT";
  }
  if (type == ZE_IMAGE_FORMAT_TYPE_SINT) {
    return "ZE_IMAGE_FORMAT_TYPE_SINT";
  }
  if (type == ZE_IMAGE_FORMAT_TYPE_UNORM) {
    return "ZE_IMAGE_FORMAT_TYPE_UNORM";
  }
  if (type == ZE_IMAGE_FORMAT_TYPE_SNORM) {
    return "ZE_IMAGE_FORMAT_TYPE_SNORM";
  }
  if (type == ZE_IMAGE_FORMAT_TYPE_FLOAT) {
    return "ZE_IMAGE_FORMAT_TYPE_FLOAT";
  }
  return "Unknown ze_image_format_type_t value: " +
         std::to_string(static_cast<int>(type));
}

std::string to_string(const ze_image_format_swizzle_t swizzle) {
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_R) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_R";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_G) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_G";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_B) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_B";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_A) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_A";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_0) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_0";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_1) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_1";
  }
  if (swizzle == ZE_IMAGE_FORMAT_SWIZZLE_X) {
    return "ZE_IMAGE_FORMAT_SWIZZLE_X";
  }
  return "Unknown ze_image_format_swizzle_t value: " +
         std::to_string(static_cast<int>(swizzle));
}

std::string to_string(const ze_image_flag_t flags) {
  std::vector<std::string> output;
  if ((flags & ZE_IMAGE_FLAG_PROGRAM_READ) != 0) {
    output.emplace_back("ZE_IMAGE_FLAG_PROGRAM_READ");
  }
  if ((flags & ZE_IMAGE_FLAG_PROGRAM_WRITE) != 0) {
    output.emplace_back("ZE_IMAGE_FLAG_PROGRAM_WRITE");
  }
  if ((flags & ZE_IMAGE_FLAG_BIAS_CACHED) != 0) {
    output.emplace_back("ZE_IMAGE_FLAG_BIAS_CACHED");
  }
  if ((flags & ZE_IMAGE_FLAG_BIAS_UNCACHED) != 0) {
    output.emplace_back("ZE_IMAGE_FLAG_BIAS_UNCACHED");
  }
  std::stringstream ss;
  for (size_t i = 0; i < output.size(); ++i) {
    ss << output[i];
    if (i < output.size() - 1) {
      ss << " | ";
    }
  }
  return ss.str();
}

std::string to_string(const ze_image_type_t type) {
  if (type == ZE_IMAGE_TYPE_1D) {
    return "ZE_IMAGE_TYPE_1D";
  }
  if (type == ZE_IMAGE_TYPE_2D) {
    return "ZE_IMAGE_TYPE_2D";
  }
  if (type == ZE_IMAGE_TYPE_3D) {
    return "ZE_IMAGE_TYPE_3D";
  }
  if (type == ZE_IMAGE_TYPE_1DARRAY) {
    return "ZE_IMAGE_TYPE_1DARRAY";
  }
  if (type == ZE_IMAGE_TYPE_2DARRAY) {
    return "ZE_IMAGE_TYPE_2DARRAY";
  }
  return "Unknown ze_image_type_t value: " +
         std::to_string(static_cast<int>(type));
}

std::string to_string(const ze_device_type_t type) {
  if (type == ZE_DEVICE_TYPE_GPU) {
    return "ZE_DEVICE_TYPE_GPU";
  }
  if (type == ZE_DEVICE_TYPE_FPGA) {
    return "ZE_DEVICE_TYPE_FPGA";
  }
  return "Unknown ze_device_type_t value: " +
         std::to_string(static_cast<int>(type));
}

std::string to_string(const ze_device_uuid_t uuid) {
  std::stringstream ss;
  ss << std::hex << std::setw(2) << std::setfill('0');
  ss << +uuid.id[15] << +uuid.id[14] << +uuid.id[13] << +uuid.id[12] << '-';
  ss << +uuid.id[11] << +uuid.id[10] << '-';
  ss << +uuid.id[9] << +uuid.id[8] << '-';
  ss << +uuid.id[7] << +uuid.id[6] << '-';
  ss << +uuid.id[5] << +uuid.id[4] << +uuid.id[3] << +uuid.id[2] << +uuid.id[1]
     << +uuid.id[0];
  return ss.str();
}

std::string to_string(const ze_memory_access_capabilities_t capabilities) {
  std::vector<std::string> output;
  if (capabilities == ZE_MEMORY_ACCESS_NONE) {
    output.emplace_back("ZE_MEMORY_ACCESS_NONE");
  } else {
    if ((capabilities & ZE_MEMORY_ACCESS) != 0) {
      output.emplace_back("ZE_MEMORY_ACCESS");
    }
    if ((capabilities & ZE_MEMORY_ATOMIC_ACCESS) != 0) {
      output.emplace_back("ZE_MEMORY_ATOMIC_ACCESS");
    }
    if ((capabilities & ZE_MEMORY_CONCURRENT_ACCESS) != 0) {
      output.emplace_back("ZE_MEMORY_CONCURRENT_ACCESS");
    }
    if ((capabilities & ZE_MEMORY_CONCURRENT_ATOMIC_ACCESS) != 0) {
      output.emplace_back("ZE_MEMORY_CONCURRENT_ATOMIC_ACCESS");
    }
  }
  std::stringstream ss;
  for (size_t i = 0; i < output.size(); ++i) {
    ss << output[i];
    if (i < output.size() - 1) {
      ss << " | ";
    }
  }
  return ss.str();
}

std::string to_string(const ze_driver_uuid_t uuid) {
  std::stringstream ss;
  ss << std::hex << std::setw(2) << std::setfill('0');
  ss << +uuid.id[15] << +uuid.id[14] << +uuid.id[13] << +uuid.id[12] << '-';
  ss << +uuid.id[11] << +uuid.id[10] << '-';
  ss << +uuid.id[9] << +uuid.id[8] << '-';
  ss << +uuid.id[7] << +uuid.id[6] << '-';
  ss << +uuid.id[5] << +uuid.id[4] << +uuid.id[3] << +uuid.id[2] << +uuid.id[1]
     << +uuid.id[0];
  return ss.str();
}

std::string to_string(const ze_native_kernel_uuid_t uuid) {
  std::stringstream ss;
  ss << std::hex << std::setw(2) << std::setfill('0');
  ss << +uuid.id[15] << +uuid.id[14] << +uuid.id[13] << +uuid.id[12] << '-';
  ss << +uuid.id[11] << +uuid.id[10] << '-';
  ss << +uuid.id[9] << +uuid.id[8] << '-';
  ss << +uuid.id[7] << +uuid.id[6] << '-';
  ss << +uuid.id[5] << +uuid.id[4] << +uuid.id[3] << +uuid.id[2] << +uuid.id[1]
     << +uuid.id[0];
  return ss.str();
}

std::string to_string(const ze_fp_capabilities_t capabilities) {
  std::vector<std::string> output;
  if (capabilities == ZE_FP_CAPS_NONE) {
    output.emplace_back("ZE_FP_CAPS_NONE");
  } else {
    if ((capabilities & ZE_FP_CAPS_DENORM) != 0) {
      output.emplace_back("ZE_FP_CAPS_DENORM");
    }
    if ((capabilities & ZE_FP_CAPS_INF_NAN) != 0) {
      output.emplace_back("ZE_FP_CAPS_INF_NAN");
    }
    if ((capabilities & ZE_FP_CAPS_ROUND_TO_NEAREST) != 0) {
      output.emplace_back("ZE_FP_CAPS_ROUND_TO_NEAREST");
    }
    if ((capabilities & ZE_FP_CAPS_ROUND_TO_ZERO) != 0) {
      output.emplace_back("ZE_FP_CAPS_ROUND_TO_ZERO");
    }
    if ((capabilities & ZE_FP_CAPS_ROUND_TO_INF) != 0) {
      output.emplace_back("ZE_FP_CAPS_ROUND_TO_INF");
    }
    if ((capabilities & ZE_FP_CAPS_FMA) != 0) {
      output.emplace_back("ZE_FP_CAPS_FMA");
    }
    if ((capabilities & ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT) != 0) {
      output.emplace_back("ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT");
    }
    if ((capabilities & ZE_FP_CAPS_SOFT_FLOAT) != 0) {
      output.emplace_back("ZE_FP_CAPS_SOFT_FLOAT");
    }
  }
  std::stringstream ss;
  for (size_t i = 0; i < output.size(); ++i) {
    ss << output[i];
    if (i < output.size() - 1) {
      ss << " | ";
    }
  }
  return ss.str();
}
} // namespace compute_samples

std::ostream &operator<<(std::ostream &os, const ze_api_version_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_result_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_desc_version_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_command_queue_flag_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_command_queue_mode_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_priority_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_desc_version_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_format_layout_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_format_type_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_format_swizzle_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_flag_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_image_type_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_device_type_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_device_uuid_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os,
                         const ze_memory_access_capabilities_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_driver_uuid_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_native_kernel_uuid_t &x) {
  return os << compute_samples::to_string(x);
}

std::ostream &operator<<(std::ostream &os, const ze_fp_capabilities_t &x) {
  return os << compute_samples::to_string(x);
}
