# VME Search
A sample application demonstrating the use of the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerate inter-frame motion estimation for video encoding.

There are 3 sub-tests: `basic_search`, `cost_heuristics`, and `larger_search` demonstrating different ways of configuring search parameters for motion estimation to achieve different quality and performance trade-offs.

More information can be found in:
* [vme_samples_overview](../../../docs/presentations/vme_samples_overview.pdf)
* [cl_intel_device_side_avc_vme_programmers_manual](../../../docs/programmer_guides/cl_intel_device_side_avc_vme_programmers_manual.pdf)

## Usage
    vme_search -s basic_search
    vme_search -s cost_heuristics_search
    vme_search -s larger_search
