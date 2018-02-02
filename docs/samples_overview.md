# Samples Overview

## I. Hello World Sample
____________
## Median filter

### Overview

A median filter implementation serving as a hello world application. Samples in this repository use the Boost Compute API as OpenCL C++ host API wrapper. This example demonstrates its basic use.

### Help
To show help message run `launcher median_filter -- -h`.

### Example
    launcher median_filter balloons_news.png output.png

## II. Subgroups Samples
____________

These samples demonstrate subgroups. Subgroups are a core feature in OpenCL 2.1, and also may be supported by the Khronos OpenCL `cl_khr_subgroups` extension or the Intel® OpenCL [`cl_intel_subgroups`](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups.txt) extension.

## 1. Subgroups Visualization Tutorial

### Overview

In this tutorial you will visualize how an NDRange is partitioned into subgroups. For more information, see the [sample README](compute_samples/applications/subgroups_visualization_tutorial/README.md).

### Help
To show help message run `launcher subgroups_visualization -- -h`.

### Example
    launcher subgroups_visualization

## 2. Subgroups Image Copy Tutorial

### Overview

In this tutorial you will use subgroup block reads and writes to optimize an image copy operation. For more information, see the [sample README](compute_samples/applications/subgroups_imagecopy_tutorial/README.md).

### Help
To show help message run `launcher subgroups_imagecopy -- -h`.

### Example
    launcher subgroups_imagecopy

## III. VME Samples
_________________

The presentation [vme_samples_overview](docs/presentations/vme_samples_overview.pdf) provides an overview of the architecture, programming model and interface for the OpenCL VME interfaces used in the VME samples. The programming guide [cl_intel_device_side_avc_vme_programmers_manual](docs/programmer_guides/cl_intel_device_side_avc_vme_programmers_manual.pdf) expounds on the background for VME. 

## 1. VME Search

### Overview

A sample application demonstrating the use of the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerating inter-frame motion estimation for video encoding.

There are 3 sub-tests: `basic_search`, `cost_heuristics`, and `larger_search` demonstrating different way of configuring the search parameters for motion estimation to achieve different quality and performance trade-offs.

### Help
To show help message run `launcher vme_search -- -h`.

### Example
    launcher vme_search -s basic_search
    launcher vme_search -s cost_heuristics_search
    launcher vme_search -s larger_search

## 2. VME Hierarchical Motion Estimation (HME)

### Overview

A sample application expanding on the `VME Search` sample, demonstrating an implementation of [3 tier hierarchical motion estimation](http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/AV0405/ZAMPOGLU/Hierarchicalestimation.html) to search larger search areas leveraging the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerating inter-frame motion estimation for each tier.

### Help
To show help message run `launcher vme_hme -- -h`.

### Example
    launcher vme_hme

## 3. VME Intra Motion Estimation

### Overview

A sample application expanding on the `VME HME` sample, demonstrating how to performance intra-frame motion estimation using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

### Help
To show help message run `launcher vme_intra -- -h`.

### Example
    launcher vme_intra

## 4. VME Wavefront Parallel Processing (WPP)

### Overview

A sample application expanding on the `VME HME` sample, demonstrating how to performance motion estimation using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) considering macro-block dependencies between macro-blocks. It introduces the concept of software scoreboard to handle dependencies between threads.

### Help
To show help message run `launcher vme_wpp -- -h`.

### Example
    launcher vme_wpp

## 5. VME Interlaced Processing

### Overview

A sample application demonstrating the processing of interlaced context using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

### Help
To show help message run `launcher vme_interlaced -- -h`.

### Example
    launcher vme_interlaced

## 6. VME VAAPI Interoperability

### Overview

A sample application demonstrating the processing of VAAPI created surfaces  using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

This is only supported for Linux platforms for which VAAPI implementations exist. The sample is currently supported on 16.5 (Media Server Studio 2017) driver.

### Help
To show help message run `launcher vme_interop -- -h`.

### Example
    launcher vme_interop

_______