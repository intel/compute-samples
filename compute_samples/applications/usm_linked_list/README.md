# usm_linked_list
Sample demonstrates how to use [Unified Shared Memory](https://github.com/intel/llvm/blob/863887687681f9fcd51b03572b2df470ebc1498f/sycl/doc/extensions/usm/cl_intel_unified_shared_memory.asciidoc) to represent pointer-based data structure like Linked List.

Based on the fact that USM allocators return pointers you can use them in standard pointer operations like:
* assignment - `a = b`;
* incrementation - `a++`;
* decrementation - `a--`;
* comparison - `a == b`;
* indirection - `*a`;
* member of pointer - `a->b`;

Please note that `device` memory is not accessible on host, so you can't dereference it.

If a kernel uses a memory allocation indirectly i.e. memory is not passed as a kernel argument then it should be specified using `clSetKernelExecInfo`. It can be done explicitly by passing all pointers as `CL_KERNEL_EXEC_INFO_​USM_PTRS_INTEL` or implicitly by setting `CL_KERNEL_EXEC_INFO_​INDIRECT_HOST/DEVICE/SHARED_ACCESS_INTEL`.
