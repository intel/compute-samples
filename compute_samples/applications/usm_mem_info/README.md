# usm_mem_info
In this sample you will find a way to get basic information about [Unified Shared Memory](https://github.com/intel/llvm/blob/863887687681f9fcd51b03572b2df470ebc1498f/sycl/doc/extensions/usm/cl_intel_unified_shared_memory.asciidoc) allocation.

Each USM pointer can be queried about:
* allocation type - `CL_MEM_ALLOC_TYPE_INTEL`;
* base pointer - `CL_MEM_ALLOC_BASE_PTR_INTEL`;
* size - `CL_MEM_ALLOC_SIZE_INTEL`;
