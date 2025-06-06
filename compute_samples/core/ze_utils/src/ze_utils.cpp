/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_utils/ze_utils.hpp"
#include "logging/logging.hpp"

#include "ze_api.h"
#include "zet_api.h"

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

std::string to_string(ze_bool_t boolean) {
  if (boolean != 0u) {
    return "true";
  }
  return "false";
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

std::string to_string(const zet_device_debug_property_flag_t flag) {
  switch (flag) {
  case ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH:
    return "ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH";
  case ZET_DEVICE_DEBUG_PROPERTY_FLAG_FORCE_UINT32:
    return "ZET_DEVICE_DEBUG_PROPERTY_FLAG_FORCE_UINT32";
  default:
    return "Unknown zet_device_debug_property_flag_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_mutable_command_list_exp_flags_t flag) {
  switch (flag) {
  case ZE_MUTABLE_COMMAND_LIST_EXP_FLAG_RESERVED:
    return "ZE_MUTABLE_COMMAND_LIST_EXP_FLAG_RESERVED";
  case ZE_MUTABLE_COMMAND_LIST_EXP_FLAG_FORCE_UINT32:
    return "ZE_MUTABLE_COMMAND_LIST_EXP_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_mutable_command_list_exp_flags_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const ze_mutable_command_exp_flag_t flag) {
  switch (flag) {
  case ZE_MUTABLE_COMMAND_EXP_FLAG_KERNEL_ARGUMENTS:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_KERNEL_ARGUMENTS";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_GROUP_COUNT:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_GROUP_COUNT";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_GROUP_SIZE:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_GROUP_SIZE";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_GLOBAL_OFFSET:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_GLOBAL_OFFSET";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_SIGNAL_EVENT:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_SIGNAL_EVENT";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_WAIT_EVENTS:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_WAIT_EVENTS";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_KERNEL_INSTRUCTION:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_KERNEL_INSTRUCTION";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_GRAPH_ARGUMENTS:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_GRAPH_ARGUMENTS";
  case ZE_MUTABLE_COMMAND_EXP_FLAG_FORCE_UINT32:
    return "ZE_MUTABLE_COMMAND_EXP_FLAG_FORCE_UINT32";
  default:
    return "Unknown ze_mutable_command_exp_flags_t value: " +
           std::to_string(static_cast<int>(flag));
  }
}

std::string to_string(const zet_metric_group_sampling_type_flag_t flag) {
  switch (flag) {
  case ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_EVENT_BASED:
    return "ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_EVENT_BASED";
  case ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_TIME_BASED:
    return "ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_TIME_BASED";
  case ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_EXP_TRACER_BASED:
    return "ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_EXP_TRACER_BASED";
  case ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_FORCE_UINT32:
    return "ZET_METRIC_GROUP_SAMPLING_TYPE_FLAG_FORCE_UINT32";
  default:
    return "Unknown zet_metric_group_sampling_type_flag_t value: " +
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

std::string to_string(const zes_engine_group_t type) {
  switch (type) {
  case ZES_ENGINE_GROUP_ALL:
    return "ZES_ENGINE_GROUP_ALL";
  case ZES_ENGINE_GROUP_COMPUTE_ALL:
    return "ZES_ENGINE_GROUP_COMPUTE_ALL";
  case ZES_ENGINE_GROUP_MEDIA_ALL:
    return "ZES_ENGINE_GROUP_MEDIA_ALL";
  case ZES_ENGINE_GROUP_COPY_ALL:
    return "ZES_ENGINE_GROUP_COPY_ALL";
  case ZES_ENGINE_GROUP_COMPUTE_SINGLE:
    return "ZES_ENGINE_GROUP_COMPUTE_SINGLE";
  case ZES_ENGINE_GROUP_RENDER_SINGLE:
    return "ZES_ENGINE_GROUP_RENDER_SINGLE";
  case ZES_ENGINE_GROUP_MEDIA_DECODE_SINGLE:
    return "ZES_ENGINE_GROUP_MEDIA_DECODE_SINGLE";
  case ZES_ENGINE_GROUP_MEDIA_ENCODE_SINGLE:
    return "ZES_ENGINE_GROUP_MEDIA_ENCODE_SINGLE";
  case ZES_ENGINE_GROUP_COPY_SINGLE:
    return "ZES_ENGINE_GROUP_COPY_SINGLE";
  case ZES_ENGINE_GROUP_MEDIA_ENHANCEMENT_SINGLE:
    return "ZES_ENGINE_GROUP_MEDIA_ENHANCEMENT_SINGLE";
  case ZES_ENGINE_GROUP_3D_SINGLE:
    return "ZES_ENGINE_GROUP_3D_SINGLE";
  case ZES_ENGINE_GROUP_3D_RENDER_COMPUTE_ALL:
    return "ZES_ENGINE_GROUP_3D_RENDER_COMPUTE_ALL";
  case ZES_ENGINE_GROUP_RENDER_ALL:
    return "ZES_ENGINE_GROUP_RENDER_ALL";
  case ZES_ENGINE_GROUP_3D_ALL:
    return "ZES_ENGINE_GROUP_3D_ALL";
  case ZES_ENGINE_GROUP_MEDIA_CODEC_SINGLE:
    return "ZES_ENGINE_GROUP_MEDIA_CODEC_SINGLE";
  case ZES_ENGINE_GROUP_FORCE_UINT32:
    return "ZES_ENGINE_GROUP_FORCE_UINT32";
  default:
    return "Unknown zes_engine_group_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

std::string to_string(zes_mem_type_t type) {
  switch (type) {
  case ZES_MEM_TYPE_HBM:
    return "ZES_MEM_TYPE_HBM";
  case ZES_MEM_TYPE_DDR:
    return "ZES_MEM_TYPE_DDR";
  case ZES_MEM_TYPE_DDR3:
    return "ZES_MEM_TYPE_DDR3";
  case ZES_MEM_TYPE_DDR4:
    return "ZES_MEM_TYPE_DDR4";
  case ZES_MEM_TYPE_DDR5:
    return "ZES_MEM_TYPE_DDR5";
  case ZES_MEM_TYPE_LPDDR:
    return "ZES_MEM_TYPE_LPDDR";
  case ZES_MEM_TYPE_LPDDR3:
    return "ZES_MEM_TYPE_LPDDR3";
  case ZES_MEM_TYPE_LPDDR4:
    return "ZES_MEM_TYPE_LPDDR4";
  case ZES_MEM_TYPE_LPDDR5:
    return "ZES_MEM_TYPE_LPDDR5";
  case ZES_MEM_TYPE_SRAM:
    return "ZES_MEM_TYPE_SRAM";
  case ZES_MEM_TYPE_L1:
    return "ZES_MEM_TYPE_L1";
  case ZES_MEM_TYPE_L3:
    return "ZES_MEM_TYPE_L3";
  case ZES_MEM_TYPE_GRF:
    return "ZES_MEM_TYPE_GRF";
  case ZES_MEM_TYPE_SLM:
    return "ZES_MEM_TYPE_SLM";
  case ZES_MEM_TYPE_GDDR4:
    return "ZES_MEM_TYPE_GDDR4";
  case ZES_MEM_TYPE_GDDR5:
    return "ZES_MEM_TYPE_GDDR5";
  case ZES_MEM_TYPE_GDDR5X:
    return "ZES_MEM_TYPE_GDDR5X";
  case ZES_MEM_TYPE_GDDR6:
    return "ZES_MEM_TYPE_GDDR6";
  case ZES_MEM_TYPE_GDDR6X:
    return "ZES_MEM_TYPE_GDDR6X";
  case ZES_MEM_TYPE_GDDR7:
    return "ZES_MEM_TYPE_GDDR7";
  default:
    return "Unknown zes_mem_type_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

std::string to_string(zes_mem_loc_t type) {
  switch (type) {
  case ZES_MEM_LOC_SYSTEM:
    return "ZES_MEM_LOC_SYSTEM";
  case ZES_MEM_LOC_DEVICE:
    return "ZES_MEM_LOC_DEVICE";
  default:
    return "Unknown zes_mem_loc_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

std::string to_string(zes_freq_domain_t type) {
  switch (type) {
  case ZES_FREQ_DOMAIN_GPU:
    return "ZES_FREQ_DOMAIN_GPU";
  case ZES_FREQ_DOMAIN_MEMORY:
    return "ZES_FREQ_DOMAIN_MEMORY";
  case ZES_FREQ_DOMAIN_MEDIA:
    return "ZES_FREQ_DOMAIN_MEDIA";
  default:
    return "Unknown zes_freq_domain_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

std::string to_string(zes_temp_sensors_t type) {
  switch (type) {
  case ZES_TEMP_SENSORS_GLOBAL:
    return "ZES_TEMP_SENSORS_GLOBAL";
  case ZES_TEMP_SENSORS_GPU:
    return "ZES_TEMP_SENSORS_GPU";
  case ZES_TEMP_SENSORS_MEMORY:
    return "ZES_TEMP_SENSORS_MEMORY";
  case ZES_TEMP_SENSORS_GLOBAL_MIN:
    return "ZES_TEMP_SENSORS_GLOBAL_MIN";
  case ZES_TEMP_SENSORS_GPU_MIN:
    return "ZES_TEMP_SENSORS_GPU_MIN";
  case ZES_TEMP_SENSORS_MEMORY_MIN:
    return "ZES_TEMP_SENSORS_MEMORY_MIN";
  case ZES_TEMP_SENSORS_GPU_BOARD:
    return "ZES_TEMP_SENSORS_GPU_BOARD";
  case ZES_TEMP_SENSORS_GPU_BOARD_MIN:
    return "ZES_TEMP_SENSORS_GPU_BOARD_MIN";
  case ZES_TEMP_SENSORS_VOLTAGE_REGULATOR:
    return "ZES_TEMP_SENSORS_VOLTAGE_REGULATOR";
  default:
    return "Unknown zes_temp_sensors_t value: " +
           std::to_string(static_cast<int>(type));
  }
}

void throw_if_failed(ze_result_t result, const std::string &function_name) {
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error(function_name + " failed: " + to_string(result));
  }
}

zes_device_handle_t
get_sysman_device_from_core_device(ze_device_handle_t device) {
  uint32_t driver_count = 0;
  auto result = zesDriverGet(&driver_count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    LOG_WARNING << "zesDriverGet failed: " + to_string(result);
    return nullptr;
  }
  if (driver_count == 0) {
    LOG_WARNING << "zesDriverGet returned 0 drivers";
    return nullptr;
  }

  std::vector<ze_driver_handle_t> drivers(driver_count);
  result = zesDriverGet(&driver_count, drivers.data());
  if (result != ZE_RESULT_SUCCESS) {
    LOG_WARNING << "zesDriverGet failed: " + to_string(result);
    return nullptr;
  }

  ze_device_properties_t properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
  result = zeDeviceGetProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetProperties");

  zes_uuid_t uuid = {};
  std::copy(properties.uuid.id, properties.uuid.id + ZE_MAX_DEVICE_UUID_SIZE,
            uuid.id);

  zes_device_handle_t sysman_device = nullptr;
  ze_bool_t on_subdevice = 0u; // onSubdevice and subdeviceId are not used
  uint32_t subdevice_id = 0;   // but function segfaults without them
  for (auto driver : drivers) {
    result = zesDriverGetDeviceByUuidExp(driver, uuid, &sysman_device,
                                         &on_subdevice, &subdevice_id);
    if (result == ZE_RESULT_SUCCESS && (sysman_device != nullptr)) {
      break;
    }
  }

  if (sysman_device == nullptr) {
    throw std::runtime_error("zesDriverGetDeviceByUuidExp did not find "
                             "matching core device uuid");
  }

  return sysman_device;
}
} // namespace compute_samples
