# VME Wavefront Parallel Processing (WPP)
A sample application expanding on the `VME HME` sample, demonstrating how to perform motion estimation using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) considering macro-block dependencies between macro-blocks. It introduces the concept of software scoreboard to handle dependencies between threads.

More information can be found in:
* [vme_samples_overview](../../../docs/presentations/vme_samples_overview.pdf)
* [cl_intel_device_side_avc_vme_programmers_manual](../../../docs/programmer_guides/cl_intel_device_side_avc_vme_programmers_manual.pdf)

## Usage
    vme_wpp
