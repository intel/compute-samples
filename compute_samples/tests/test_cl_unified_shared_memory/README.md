# test_cl_unified_shared_memory

## Overview
Tests for [cl_intel_unified_shared_memory](https://github.com/intel/llvm/blob/sycl/sycl/doc/extensions/USM/cl_intel_unified_shared_memory.asciidoc) extension which adds Unified Shared Memory to OpenCL.
USM represents OpenCL allocations as pointers rather than handles and allows fine-grain control over ownership and accessibility of OpenCL allocations.

## Tests
1. Basic
    - [x] Allocate memory using `clHostMemAllocINTEL`.
        - [x] Different `cl_mem_properties_intel`.
        - [x] Different alignments.
    - [x] Allocate memory using `clDeviceMemAllocINTEL`.
    - [x] Allocate memory using `clSharedMemAllocINTEL`.
    - [x] Free different USM allocation types using `clMemFreeINTEL`.
    - [ ] Free different USM allocation types using `clMemBlockingFreeINTEL`.
    - [x] Query information about different USM allocation types using `clGetMemAllocInfoINTEL`.
        - [x] `CL_MEM_ALLOC_TYPE_INTEL`.
        - [x] `CL_MEM_ALLOC_FLAGS_INTEL`.
        - [x] `CL_MEM_ALLOC_BASE_PTR_INTEL`.
        - [x] `CL_MEM_ALLOC_ALLOC_SIZE_INTEL`.
    - [x] Set different USM allocation types as kernel argument using `clSetKernelArgMemPointerINTEL`.
        - [x] Different offsets.
    - [x] Enqueue memory fill on different USM allocation types using `clEnqueueMemFillINTEL`.
        - [x] Different allocation sizes.
        - [x] Different region sizes.
        - [x] Different offsets.
    - [x] Enqueue memory copy of different USM allocation types using `clEnqueueMemcpyINTEL`.
        - [x] Different allocation sizes.
        - [x] Different region sizes.
        - [x] Different source offsets.
        - [x] Different destination offsets.
        - [x] Blocking and non-blocking call.
    - [x] Enqueue memory migration of different USM allocation types using `clEnqueueMigrateMemINTEL`.
        - [x] Different offsets.
    - [x] Enqueue memory advise of different USM allocation types using `clEnqueueMemAdviseINTEL`.
        - [x] Different offsets.
    - [x] Verify that USM allocations are accessible on both host and device.
    - [x] Mix USM allocation types in the same OpenCL C program.
    - [x] Verify atomic operations on different USM types.
    - [x] Query device information about USM using `clGetDeviceInfo`.
        - [x] `CL_DEVICE_HOST_MEM_CAPABILITIES_INTEL`.
        - [x] `CL_DEVICE_DEVICE_MEM_CAPABILITIES_INTEL`.
        - [x] `CL_DEVICE_SINGLE_DEVICE_SHARED_MEM_CAPABILITIES_INTEL`.
        - [x] `CL_DEVICE_CROSS_DEVICE_SHARED_MEM_CAPABILITIES_INTEL`.
        - [x] `CL_DEVICE_SHARED_SYSTEM_MEM_CAPABILITIES_INTEL`.
    - [x] Verify that extension is supported by a device.
    - [x] Verify indirect allocations explicitly and implicitly using `clSetKernelExecInfo`.
        - [x] `CL_KERNEL_EXEC_INFO_USM_PTRS_INTEL`.
        - [x] `CL_KERNEL_EXEC_INFO_INDIRECT_HOST_ACCESS_INTEL`.
        - [x] `CL_KERNEL_EXEC_INFO_INDIRECT_DEVICE_ACCESS_INTEL`.
        - [x] `CL_KERNEL_EXEC_INFO_INDIRECT_SHARED_ACCESS_INTEL`.
    - [x] Query event type returned from USM API using `clGetEventInfo`.
    - [x] Verify that event wait lists are properly handled by new functions.
    - [x] Verify that events returned by new functions can be used as an input to standard OpenCL functions.
    - [x] Verify Shared System allocations.
    - [ ] Interoperability with OpenCL buffers i.e. `clCreateBuffer`, `clEnqueueReadBuffer`, `clEnqueueWriteBuffer`.
    - [ ] Interoperability with OpenCL images i.e. `clCreateImage`, `clEnqueueReadImage`, `clEnqueueWriteImage`.
1. Advanced
    - [ ] Verify concurrent access to USM allocations.
    - [ ] Verify oversubscription of USM allocations.
    - [ ] Verify peer-to-peer access.

## Selecting tests
* To list all of available test cases please use `--gtest_list_tests` flag.
* To run only selected test case please use `--gtest_filter` flag.

More details can be found in [Google Test documentation]([https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#selecting-tests).
