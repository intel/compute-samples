# usm_queries
Sample demonstrating how to query OpenCL device capabilities related to [Unified Shared Memory](https://github.com/intel/llvm/blob/863887687681f9fcd51b03572b2df470ebc1498f/sycl/doc/extensions/usm/cl_intel_unified_shared_memory.asciidoc).

To check if your device supports Unified Shared Memory you should verify if `cl_intel_unified_shared_memory_preview` extension string is present in `CL_DEVICE_EXTENSIONS`.

Unified Shared Memory specification defines five different allocation types that you can use:
1. `Host`;
1. `Device`;
1. `Shared`;
1. `Shared (Cross-Device)`;
1. `Shared System (Cross-Device)`;

Device can allow different set of operations for each allocation type, so you should check supported USM capabilities:
* `MEMORY_ACCESS`;
* `MEMORY_ATOMIC_ACCESS`;
* `MEMORY_CONCURRENT_ACCESS`;
* `MEMORY_CONCURRENT_ATOMIC_ACCESS`;

## Usage
    usm_queries
