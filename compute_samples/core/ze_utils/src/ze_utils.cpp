/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_utils/ze_utils.hpp"
#include "logging/logging.hpp"

#include "ze_api.h"

#include <stdexcept>
#include <string>
#include <iomanip>

namespace compute_samples {
std::string to_string(const ze_result_t result) {
  switch (result) {
  case ZE_RESULT_SUCCESS:
    return "ZE_RESULT_SUCCESS";
  case ZE_RESULT_NOT_READY:
    return "ZE_RESULT_NOT_READY";
  case ZE_RESULT_ERROR_DEVICE_LOST:
    return "ZE_RESULT_ERROR_DEVICE_LOST";
  case ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY:
    return "ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY";
  case ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY:
    return "ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY";
  case ZE_RESULT_ERROR_MODULE_BUILD_FAILURE:
    return "ZE_RESULT_ERROR_MODULE_BUILD_FAILURE";
  case ZE_RESULT_ERROR_MODULE_LINK_FAILURE:
    return "ZE_RESULT_ERROR_MODULE_LINK_FAILURE";
  case ZE_RESULT_ERROR_DEVICE_REQUIRES_RESET:
    return "ZE_RESULT_ERROR_DEVICE_REQUIRES_RESET";
  case ZE_RESULT_ERROR_DEVICE_IN_LOW_POWER_STATE:
    return "ZE_RESULT_ERROR_DEVICE_IN_LOW_POWER_STATE";
  case ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS:
    return "ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS";
  case ZE_RESULT_ERROR_NOT_AVAILABLE:
    return "ZE_RESULT_ERROR_NOT_AVAILABLE";
  case ZE_RESULT_ERROR_DEPENDENCY_UNAVAILABLE:
    return "ZE_RESULT_ERROR_DEPENDENCY_UNAVAILABLE";
  case ZE_RESULT_ERROR_UNINITIALIZED:
    return "ZE_RESULT_ERROR_UNINITIALIZED";
  case ZE_RESULT_ERROR_UNSUPPORTED_VERSION:
    return "ZE_RESULT_ERROR_UNSUPPORTED_VERSION";
  case ZE_RESULT_ERROR_UNSUPPORTED_FEATURE:
    return "ZE_RESULT_ERROR_UNSUPPORTED_FEATURE";
  case ZE_RESULT_ERROR_INVALID_ARGUMENT:
    return "ZE_RESULT_ERROR_INVALID_ARGUMENT";
  case ZE_RESULT_ERROR_INVALID_NULL_HANDLE:
    return "ZE_RESULT_ERROR_INVALID_NULL_HANDLE";
  case ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE:
    return "ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE";
  case ZE_RESULT_ERROR_INVALID_NULL_POINTER:
    return "ZE_RESULT_ERROR_INVALID_NULL_POINTER";
  case ZE_RESULT_ERROR_INVALID_SIZE:
    return "ZE_RESULT_ERROR_INVALID_SIZE";
  case ZE_RESULT_ERROR_UNSUPPORTED_SIZE:
    return "ZE_RESULT_ERROR_UNSUPPORTED_SIZE";
  case ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT:
    return "ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT";
  case ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT:
    return "ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT";
  case ZE_RESULT_ERROR_INVALID_ENUMERATION:
    return "ZE_RESULT_ERROR_INVALID_ENUMERATION";
  case ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION:
    return "ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION";
  case ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT:
    return "ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT";
  case ZE_RESULT_ERROR_INVALID_NATIVE_BINARY:
    return "ZE_RESULT_ERROR_INVALID_NATIVE_BINARY";
  case ZE_RESULT_ERROR_INVALID_GLOBAL_NAME:
    return "ZE_RESULT_ERROR_INVALID_GLOBAL_NAME";
  case ZE_RESULT_ERROR_INVALID_KERNEL_NAME:
    return "ZE_RESULT_ERROR_INVALID_KERNEL_NAME";
  case ZE_RESULT_ERROR_INVALID_FUNCTION_NAME:
    return "ZE_RESULT_ERROR_INVALID_FUNCTION_NAME";
  case ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION:
    return "ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION";
  case ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION:
    return "ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION";
  case ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX:
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX";
  case ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE:
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE";
  case ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE:
    return "ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE";
  case ZE_RESULT_ERROR_INVALID_MODULE_UNLINKED:
    return "ZE_RESULT_ERROR_INVALID_MODULE_UNLINKED";
  case ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE:
    return "ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE";
  case ZE_RESULT_ERROR_OVERLAPPING_REGIONS:
    return "ZE_RESULT_ERROR_OVERLAPPING_REGIONS";
  case ZE_RESULT_ERROR_UNKNOWN:
    return "ZE_RESULT_ERROR_UNKNOWN";
  case ZE_RESULT_FORCE_UINT32:
    return "ZE_RESULT_FORCE_UINT32";
  default:
    return "Unknown ze_result_t value: " +
           std::to_string(static_cast<int>(result));
  }
}

std::string to_string(const ze_device_memory_property_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD:
    return "ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD";
  case ZE_DEVICE_MEMORY_PROPERTY_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_MEMORY_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_memory_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_ipc_property_flag_t flag) {
  switch (flag) {
  case ZE_IPC_PROPERTY_FLAG_MEMORY:
    return "ZE_IPC_PROPERTY_FLAG_MEMORY";
  case ZE_IPC_PROPERTY_FLAG_EVENT_POOL:
    return "ZE_IPC_PROPERTY_FLAG_EVENT_POOL";
  case ZE_IPC_PROPERTY_FLAG_FORCE_UINT32:
    return "ZE_IPC_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_ipc_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_type_t type) {
  switch (type) {
  case ZE_DEVICE_TYPE_GPU:
    return "ZE_DEVICE_TYPE_GPU";
  case ZE_DEVICE_TYPE_CPU:
    return "ZE_DEVICE_TYPE_CPU";
  case ZE_DEVICE_TYPE_FPGA:
    return "ZE_DEVICE_TYPE_FPGA";
  case ZE_DEVICE_TYPE_MCA:
    return "ZE_DEVICE_TYPE_MCA";
  case ZE_DEVICE_TYPE_VPU:
    return "ZE_DEVICE_TYPE_VPU";
  case ZE_DEVICE_TYPE_FORCE_UINT32:
    return "ZE_DEVICE_TYPE_FORCE_UINT32";
  default:
    return "Unknown ze_device_type_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

std::string to_string(const ze_memory_access_cap_flag_t flag) {
  switch (flag) {
  case ZE_MEMORY_ACCESS_CAP_FLAG_RW:
    return "ZE_MEMORY_ACCESS_CAP_FLAG_RW";
  case ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC:
    return "ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC";
  case ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT:
    return "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT";
  case ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC:
    return "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC";
  case ZE_MEMORY_ACCESS_CAP_FLAG_FORCE_UINT32:
    return "ZE_MEMORY_ACCESS_CAP_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_memory_access_cap_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_external_memory_type_flag_t flag) {
  switch (flag) {
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_WIN32:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_WIN32";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_WIN32_KMT:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_WIN32_KMT";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D11_TEXTURE:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D11_TEXTURE";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D11_TEXTURE_KMT:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D11_TEXTURE_KMT";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D12_HEAP:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D12_HEAP";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D12_RESOURCE:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_D3D12_RESOURCE";
  case ZE_EXTERNAL_MEMORY_TYPE_FLAG_FORCE_UINT32:
    return "ZE_EXTERNAL_MEMORY_TYPE_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_external_memory_type_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_cache_property_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL:
    return "ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL";
  case ZE_DEVICE_CACHE_PROPERTY_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_CACHE_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_cache_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_property_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_PROPERTY_FLAG_INTEGRATED:
    return "ZE_DEVICE_PROPERTY_FLAG_INTEGRATED";
  case ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE:
    return "ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE";
  case ZE_DEVICE_PROPERTY_FLAG_ECC:
    return "ZE_DEVICE_PROPERTY_FLAG_ECC";
  case ZE_DEVICE_PROPERTY_FLAG_ONDEMANDPAGING:
    return "ZE_DEVICE_PROPERTY_FLAG_ONDEMANDPAGING";
  case ZE_DEVICE_PROPERTY_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_module_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_MODULE_FLAG_FP16:
    return "ZE_DEVICE_MODULE_FLAG_FP16";
  case ZE_DEVICE_MODULE_FLAG_FP64:
    return "ZE_DEVICE_MODULE_FLAG_FP64";
  case ZE_DEVICE_MODULE_FLAG_INT64_ATOMICS:
    return "ZE_DEVICE_MODULE_FLAG_INT64_ATOMICS";
  case ZE_DEVICE_MODULE_FLAG_DP4A:
    return "ZE_DEVICE_MODULE_FLAG_DP4A";
  case ZE_DEVICE_MODULE_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_MODULE_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_module_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_fp_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_FP_FLAG_DENORM:
    return "ZE_DEVICE_FP_FLAG_DENORM";
  case ZE_DEVICE_FP_FLAG_INF_NAN:
    return "ZE_DEVICE_FP_FLAG_INF_NAN";
  case ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST:
    return "ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST";
  case ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO:
    return "ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO";
  case ZE_DEVICE_FP_FLAG_ROUND_TO_INF:
    return "ZE_DEVICE_FP_FLAG_ROUND_TO_INF";
  case ZE_DEVICE_FP_FLAG_FMA:
    return "ZE_DEVICE_FP_FLAG_FMA";
  case ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT:
    return "ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT";
  case ZE_DEVICE_FP_FLAG_SOFT_FLOAT:
    return "ZE_DEVICE_FP_FLAG_SOFT_FLOAT";
  case ZE_DEVICE_FP_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_FP_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_fp_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_command_queue_group_property_flag_t flag) {
  switch (flag) {
  case ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE:
    return "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE";
  case ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY:
    return "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COPY";
  case ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS:
    return "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS";
  case ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_METRICS:
    return "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_METRICS";
  case ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_FORCE_UINT32:
    return "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_command_queue_group_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_scheduling_hint_exp_flag_t flag) {
  switch (flag) {
  case ZE_SCHEDULING_HINT_EXP_FLAG_OLDEST_FIRST:
    return "ZE_SCHEDULING_HINT_EXP_FLAG_OLDEST_FIRST";
  case ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN:
    return "ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN";
  case ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN:
    return "ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN";
  case ZE_SCHEDULING_HINT_EXP_FLAG_FORCE_UINT32:
    return "ZE_SCHEDULING_HINT_EXP_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_scheduling_hint_exp_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_fp_atomic_ext_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_LOAD_STORE:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_LOAD_STORE";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_ADD:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_ADD";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_MIN_MAX:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_LOCAL_MIN_MAX";
  case ZE_DEVICE_FP_ATOMIC_EXT_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_device_fp_atomic_ext_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_device_raytracing_ext_flag_t flag) {
  switch (flag) {
  case ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY:
    return "ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY";
  case ZE_DEVICE_RAYTRACING_EXT_FLAG_FORCE_UINT32:
    return "ZE_DEVICE_RAYTRACING_EXT_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_raytracing_mem_alloc_ext_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

void throw_if_failed(ze_result_t result, const std::string &function_name) {
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error(function_name + " failed: " + to_string(result));
  }
}

} // namespace compute_samples
