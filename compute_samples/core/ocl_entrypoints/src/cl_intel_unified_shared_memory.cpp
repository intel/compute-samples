/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ocl_entrypoints/cl_intel_unified_shared_memory.h"
#include "ocl_entrypoints/loader.hpp"

namespace cs = compute_samples;

CL_API_ENTRY void *CL_API_CALL clHostMemAllocINTEL(
    cl_context context, const cl_mem_properties_intel *properties, size_t size,
    cl_uint alignment, cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clHostMemAllocINTEL_fn>(
      context, "clHostMemAllocINTEL");
  return e(context, properties, size, alignment, errcodeRet);
}

CL_API_ENTRY void *CL_API_CALL clDeviceMemAllocINTEL(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clDeviceMemAllocINTEL_fn>(
      context, "clDeviceMemAllocINTEL");
  return e(context, device, properties, size, alignment, errcodeRet);
}

CL_API_ENTRY void *CL_API_CALL clSharedMemAllocINTEL(
    cl_context context, cl_device_id device,
    const cl_mem_properties_intel *properties, size_t size, cl_uint alignment,
    cl_int *errcodeRet) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clSharedMemAllocINTEL_fn>(
      context, "clSharedMemAllocINTEL");
  return e(context, device, properties, size, alignment, errcodeRet);
}

CL_API_ENTRY cl_int CL_API_CALL
clMemFreeINTEL(cl_context context, const void *ptr) CL_API_SUFFIX__VERSION_2_2 {
  const auto e =
      cs::load_entrypoint<clMemFreeINTEL_fn>(context, "clMemFreeINTEL");
  return e(context, ptr);
}

CL_API_ENTRY cl_int CL_API_CALL clGetMemAllocInfoINTEL(
    cl_context context, const void *ptr, cl_mem_info_intel paramName,
    size_t paramValueSize, void *paramValue,
    size_t *paramValueSizeRet) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clGetMemAllocInfoINTEL_fn>(
      context, "clGetMemAllocInfoINTEL");
  return e(context, ptr, paramName, paramValueSize, paramValue,
           paramValueSizeRet);
}

CL_API_ENTRY cl_int CL_API_CALL
clSetKernelArgMemPointerINTEL(cl_kernel kernel, cl_uint argIndex,
                              const void *argValue) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clSetKernelArgMemPointerINTEL_fn>(
      kernel, "clSetKernelArgMemPointerINTEL");
  return e(kernel, argIndex, argValue);
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemsetINTEL(
    cl_command_queue commandQueue, void *dstPtr, cl_int value, size_t size,
    cl_uint numEventsInWaitList, const cl_event *eventWaitList,
    cl_event *event) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clEnqueueMemsetINTEL_fn>(
      commandQueue, "clEnqueueMemsetINTEL");
  return e(commandQueue, dstPtr, value, size, numEventsInWaitList,
           eventWaitList, event);
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemcpyINTEL(
    cl_command_queue commandQueue, cl_bool blocking, void *dstPtr,
    const void *srcPtr, size_t size, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clEnqueueMemcpyINTEL_fn>(
      commandQueue, "clEnqueueMemcpyINTEL");
  return e(commandQueue, blocking, dstPtr, srcPtr, size, numEventsInWaitList,
           eventWaitList, event);
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueMigrateMemINTEL(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_migration_flags flags, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clEnqueueMigrateMemINTEL_fn>(
      commandQueue, "clEnqueueMigrateMemINTEL");
  return e(commandQueue, ptr, size, flags, numEventsInWaitList, eventWaitList,
           event);
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueMemAdviseINTEL(
    cl_command_queue commandQueue, const void *ptr, size_t size,
    cl_mem_advice_intel advice, cl_uint numEventsInWaitList,
    const cl_event *eventWaitList, cl_event *event) CL_API_SUFFIX__VERSION_2_2 {
  const auto e = cs::load_entrypoint<clEnqueueMemAdviseINTEL_fn>(
      commandQueue, "clEnqueueMemAdviseINTEL");
  return e(commandQueue, ptr, size, advice, numEventsInWaitList, eventWaitList,
           event);
}
