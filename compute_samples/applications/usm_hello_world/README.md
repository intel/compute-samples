# usm_hello_world
This sample shows basics of [Unified Shared Memory](https://github.com/intel/llvm/blob/863887687681f9fcd51b03572b2df470ebc1498f/sycl/doc/extensions/usm/cl_intel_unified_shared_memory.asciidoc). Application implements a simple copy buffer kernel and demonstrates essential operations related to the USM:
* Memory allocation
* Memory reads and writes
* Passing USM to OpenCL kernels

Source code is organized in a way to highlight similarities and differences between USM types: `host`, `device` and `shared`. To change used USM type just pass its name from a command line as an positional argument.

It is also possible to change allocation size simply by passing `--size N` option from a command line. `N` is a number of elements to allocate and each element has 4 bytes (`cl_uint`).

Basic structure of an application using USM is presented below:
1. Check if `cl_intel_unified_shared_memory_preview` extension is reported by a device.
1. Check if all required USM capabilities are supported.
1. Allocate memory using:
    1. `clHostMemAllocINTEL`;
    1. `clDeviceMemAllocINTEL`;
    1. `clSharedMemAllocINTEL`;
    1. `malloc`/`new`;
1. Populate memory using for example:
    1. `memcpy`/`std::copy`/`clEnqueueMemcpyINTEL`;
    1. `memset`/`std::fill`/`clEnqueueMemsetINTEL`;
1. Create kernel.
1. Set memory as kernel argument using `clSetKernelArgMemPointerINTEL`.
1. Run kernel.
1. Read results using for example:
    1. `memcpy`/`std::copy`/`clEnqueueMemcpyINTEL`;
1. Free memory using `clMemFreeINTEL`.

## Usage
    usm_hello_world host
    usm_hello_world device
    usm_hello_world shared
    usm_hello_world host --size 1024
