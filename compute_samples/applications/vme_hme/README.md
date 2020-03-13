# VME Hierarchical Motion Estimation (HME)
A sample application expanding on the `VME Search` sample, demonstrating an implementation of [3 tier hierarchical motion estimation](http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/AV0405/ZAMPOGLU/Hierarchicalestimation.html) to search larger search areas. Leveraging the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerate inter-frame motion estimation for each tier.

More information can be found in:
* [vme_samples_overview](../../../docs/presentations/vme_samples_overview.pdf)
* [cl_intel_device_side_avc_vme_programmers_manual](../../../docs/programmer_guides/cl_intel_device_side_avc_vme_programmers_manual.pdf)

## Usage
    vme_hme
