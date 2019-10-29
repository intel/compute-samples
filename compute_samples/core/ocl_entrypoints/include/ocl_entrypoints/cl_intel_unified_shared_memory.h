/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CL_INTEL_UNIFIED_SHARED_MEMORY_H
#define CL_INTEL_UNIFIED_SHARED_MEMORY_H

#include <CL/cl.h>

#define CL_DEVICE_HOST_MEM_CAPABILITIES_INTEL 0x4190
#define CL_DEVICE_DEVICE_MEM_CAPABILITIES_INTEL 0x4191
#define CL_DEVICE_SINGLE_DEVICE_SHARED_MEM_CAPABILITIES_INTEL 0x4192
#define CL_DEVICE_CROSS_DEVICE_SHARED_MEM_CAPABILITIES_INTEL 0x4193
#define CL_DEVICE_SHARED_SYSTEM_MEM_CAPABILITIES_INTEL 0x4194

typedef cl_bitfield cl_unified_shared_memory_capabilities_intel;

#define CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL 1u
#define CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL 2u
#define CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL 4u
#define CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL 8u

typedef cl_bitfield cl_mem_properties_intel;

#define CL_MEM_ALLOC_FLAGS_INTEL 0x4195

typedef cl_bitfield cl_mem_alloc_flags_intel;

#define CL_MEM_ALLOC_DEFAULT_INTEL 0
#define CL_MEM_ALLOC_WRITE_COMBINED_INTEL (1 << 0)

typedef cl_uint cl_mem_info_intel;

#define CL_MEM_ALLOC_TYPE_INTEL 0x419A
#define CL_MEM_ALLOC_BASE_PTR_INTEL 0x419B
#define CL_MEM_ALLOC_SIZE_INTEL 0x419C
#define CL_MEM_ALLOC_INFO_TBD0_INTEL 0x419D /* reserved for future */
#define CL_MEM_ALLOC_INFO_TBD1_INTEL 0x419E /* reserved for future */
#define CL_MEM_ALLOC_INFO_TBD2_INTEL 0x419F /* reserved for future */

typedef cl_uint cl_unified_shared_memory_type_intel;

#define CL_MEM_TYPE_UNKNOWN_INTEL 0x4196
#define CL_MEM_TYPE_HOST_INTEL 0x4197
#define CL_MEM_TYPE_DEVICE_INTEL 0x4198
#define CL_MEM_TYPE_SHARED_INTEL 0x4199

typedef cl_uint cl_mem_advice_intel;

#define CL_MEM_ADVICE_TBD0_INTEL 0x4208 /* reserved for future */
#define CL_MEM_ADVICE_TBD1_INTEL 0x4209 /* reserved for future */
#define CL_MEM_ADVICE_TBD2_INTEL 0x420A /* reserved for future */
#define CL_MEM_ADVICE_TBD3_INTEL 0x420B /* reserved for future */
#define CL_MEM_ADVICE_TBD4_INTEL 0x420C /* reserved for future */
#define CL_MEM_ADVICE_TBD5_INTEL 0x420D /* reserved for future */
#define CL_MEM_ADVICE_TBD6_INTEL 0x420E /* reserved for future */
#define CL_MEM_ADVICE_TBD7_INTEL 0x420F /* reserved for future */

#define CL_KERNEL_EXEC_INFO_INDIRECT_HOST_ACCESS_INTEL 0x4200
#define CL_KERNEL_EXEC_INFO_INDIRECT_DEVICE_ACCESS_INTEL 0x4201
#define CL_KERNEL_EXEC_INFO_INDIRECT_SHARED_ACCESS_INTEL 0x4202

#define CL_KERNEL_EXEC_INFO_USM_PTRS_INTEL 0x4203

#define CL_COMMAND_MEMSET_INTEL 0x4204
#define CL_COMMAND_MEMCPY_INTEL 0x4205
#define CL_COMMAND_MIGRATEMEM_INTEL 0x4206
#define CL_COMMAND_MEMADVISE_INTEL 0x4207

#define CL_MEM_ALLOC_TYPE_INTEL 0x419A
#define CL_MEM_ALLOC_BASE_PTR_INTEL 0x419B
#define CL_MEM_ALLOC_SIZE_INTEL 0x419C

typedef CL_API_ENTRY void *(CL_API_CALL *clHostMemAllocINTEL_fn)(
    cl_context context, const cl_mem_properties_intel *properties, size_t size,
    cl_uint alignment, cl_int *errcodeRet)CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY void *CL_API_CALL clHostMemAllocINTEL(
    cl_context context, const cl_mem_properties_intel *properties, size_t size,
    cl_uint alignment, cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY void *(CL_API_CALL *clDeviceMemAllocINTEL_fn)(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet)CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY void *CL_API_CALL clDeviceMemAllocINTEL(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY void *(CL_API_CALL *clSharedMemAllocINTEL_fn)(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet)CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY void *CL_API_CALL clSharedMemAllocINTEL(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clMemFreeINTEL_fn)(
    cl_context context, const void *ptr) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL
clMemFreeINTEL(cl_context context, const void *ptr) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clGetMemAllocInfoINTEL_fn)(
    cl_context context, const void *ptr, cl_mem_info_intel paramName,
    size_t paramValueSize, void *paramValue,
    size_t *paramValueSizeRet) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL clGetMemAllocInfoINTEL(
    cl_context context, const void *ptr, cl_mem_info_intel paramName,
    size_t paramValueSize, void *paramValue,
    size_t *paramValueSizeRet) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clSetKernelArgMemPointerINTEL_fn)(
    cl_kernel kernel, cl_uint argIndex,
    const void *argValue) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArgMemPointerINTEL(cl_kernel kernel, cl_uint argIndex,
                              const void *argValue) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clEnqueueMemsetINTEL_fn)(
    cl_command_queue commandQueue, void *dstPtr, cl_int value, size_t size,
    cl_uint numEventsInWaitList, const cl_event *eventWaitList,
    cl_event *event) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemsetINTEL(
    cl_command_queue commandQueue, void *dstPtr, cl_int value, size_t size,
    cl_uint numEventsInWaitList, const cl_event *eventWaitList,
    cl_event *event) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clEnqueueMemcpyINTEL_fn)(
    cl_command_queue commandQueue, cl_bool blocking, void *dstPtr,
    const void *srcPtr, size_t size, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemcpyINTEL(
    cl_command_queue commandQueue, cl_bool blocking, void *dstPtr,
    const void *srcPtr, size_t size, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clEnqueueMigrateMemINTEL_fn)(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_migration_flags flags, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL clEnqueueMigrateMemINTEL(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_migration_flags flags, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

typedef CL_API_ENTRY cl_int(CL_API_CALL *clEnqueueMemAdviseINTEL_fn)(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_advice_intel advice, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

extern CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemAdviseINTEL(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_advice_intel advice, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2;

#endif
