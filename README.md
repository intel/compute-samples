# Intel® GPU Compute Samples
Sample GPU Compute applications for Intel® Processor Graphics that demonstrate: 

- notable new features in Compute APIs in released Graphics drivers
- features specific to Intel® Processor Graphics   
- efficient way of using Intel® GPUs for general computing

Compute samples are currently limited to the OpenCL™ compute API.

## Getting Started

### Prerequisites
* [Intel® Graphics 5XX (Skylake) or newer](https://en.wikipedia.org/wiki/Intel_HD_and_Iris_Graphics)
* [OpenCL GPU driver](https://software.intel.com/en-us/articles/opencl-drivers)
* Compiler with C++11 support
  * [GCC 4.8.5](https://gcc.gnu.org/) or newer 
  * [Microsoft Visual Studio 2015](https://www.visualstudio.com/) or newer
  * [Clang 3.5](https://clang.llvm.org/) or newer
* [CMake 3.8](https://cmake.org/) or newer
* Linux specific
  * [libva, libva-drm, libva-x11 (VAAPI runtime libraries for Linux)](https://01.org/linuxgraphics/community/vaapi)
  * [libX11 (X Windows client library)](https://www.x.org)

### Build
CMake requires properly configured proxy settings using environmental variables: `HTTP_PROXY` and `HTTPS_PROXY`.
First build can take few minutes because all dependencies are set.

#### Windows
    git clone https://github.com/intel/compute-samples
    cd compute-samples
    mkdir build
    cd build
    cmake .. -G "Visual Studio 14 2015"
    Open compute_samples.sln
    Build projects
    
If you have issues with building Boost with Visual Studio 2017 then please execute above commands using `Developer Command Prompt For VS 2017`.
    
#### Linux
    git clone https://github.com/intel/compute-samples
    cd compute-samples
    mkdir build
    cd build
    cmake ..
    make
    
### Installation
The following commands install project's binaries.
By default they are installed in `build/bin` directory, but it can be customized by adding `CMAKE_INSTALL_PREFIX=your/path` during CMake configuration.

#### Windows
    Open compute_samples.sln
    Build INSTALL project
    
#### Linux
    make install

### Test
There are 3 ways to run tests depending on the use case:

* To run all registered tests from all modules run `ctest` from the `build` directory.
* To run all tests from a specific module find module's tester and run it.
* To run specific tests from a specific module find module's tester and run it with `--gtest_filter` option or using the `ctest -R *test_pattern*` option.

### Dependencies
* [Boost 1.64](http://www.boost.org/)
* [libpng 1.6.30](http://www.libpng.org/pub/png/libpng.html)

### CMake options
CMake can be configured using the following parameters:

* `BUILD_BOOST` - If set to `OFF` then Boost won't be downloaded and built during CMake configuration. Default is `ON`. Please note that most of the time to build the project the first time is in downloading and building Boost.
* `BOOST_ROOT` - Path to the custom Boost installation directory. It is only relevant if `BUILD_BOOST` is set to `OFF`.
* `BUILD_PNG` - If set to `OFF` then libpng won't be downloaded and built during CMake configuration. Default is `ON`.
* `DOWNLOAD_MEDIA` - If set to `OFF` media files to use as inputs for sample application won't be downloaded. Default in `ON`.

Example command line: `cmake .. -DBUILD_BOOST=OFF -DBOOST_ROOT=/home/boost_1_64_0 -DBUILD_PNG=OFF -DDOWNLOAD_MEDIA=OFF`
    
## Versioning
We use [Semantic Versioning](http://semver.org/). Current version is `0.y.z` which means that API may change without maintaining backward compatibility.

## Contributing
Please read [CONTRIBUTING](CONTRIBUTING.md) for details about the contribution process.

## License
Intel® GPU Compute Samples are licensed under the [MIT License](LICENSE).

## Samples Overview

### I. Hello World Sample
____________
### Median filter

#### Overview

A median filter implementation serving as a hello world application. Samples in this repository use the Boost Compute API as OpenCL C++ host API wrapper. This example demonstrates its basic use.

#### Help
To show help message run `launcher median_filter -- -h`.

#### Example
    launcher median_filter balloons_news.png output.png

### II. Subgroups Samples
____________

These samples demonstrate subgroups. Subgroups are a core feature in OpenCL 2.1, and also may be supported by the Khronos OpenCL `cl_khr_subgroups` extension or the Intel® OpenCL [`cl_intel_subgroups`](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups.txt) extension.

### 1. Subgroups Visualization Tutorial

#### Overview

In this tutorial you will visualize how an NDRange is partitioned into subgroups. For more information, see the [sample README](compute_samples/applications/subgroups_visualization_tutorial/README.md).

#### Help
To show help message run `launcher subgroups_visualization -- -h`.

#### Example
    launcher subgroups_visualization

### 2. Subgroups Image Copy Tutorial

#### Overview

In this tutorial you will use subgroup block reads and writes to optimize an image copy operation. For more information, see the [sample README](compute_samples/applications/subgroups_imagecopy_tutorial/README.md).

#### Help
To show help message run `launcher subgroups_imagecopy -- -h`.

#### Example
    launcher subgroups_imagecopy

### III. VME Samples
_________________

The presentation [vme_samples_overview](docs/presentations/vme_samples_overview.pdf) provides an overview of the architecture, programming model and interface for the OpenCL VME interfaces used in the VME samples. The programming guide [cl_intel_device_side_avc_vme_programmers_manual](docs/programmer_guides/cl_intel_device_side_avc_vme_programmers_manual.pdf) expounds on the background for VME. 

### 1. VME Search

#### Overview

A sample application demonstrating the use of the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerating inter-frame motion estimation for video encoding.

There are 3 sub-tests: `basic_search`, `cost_heuristics`, and `larger_search` demonstrating different way of configuring the search parameters for motion estimation to achieve different quality and performance trade-offs.

#### Help
To show help message run `launcher vme_search -- -h`.

#### Example
    launcher vme_search -s basic_search
    launcher vme_search -s cost_heuristics_search
    launcher vme_search -s larger_search

### 2. VME Hierarchical Motion Estimation (HME)

#### Overview

A sample application expanding on the `VME Search` sample, demonstrating an implementation of [3 tier hierarchical motion estimation](http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/AV0405/ZAMPOGLU/Hierarchicalestimation.html) to search larger search areas leveraging the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) to accelerating inter-frame motion estimation for each tier.

#### Help
To show help message run `launcher vme_hme -- -h`.

#### Example
    launcher vme_hme

### 3. VME Intra Motion Estimation

#### Overview

A sample application expanding on the `VME HME` sample, demonstrating how to performance intra-frame motion estimation using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

#### Help
To show help message run `launcher vme_intra -- -h`.

#### Example
    launcher vme_intra

### 4. VME Wavefront Parallel Processing (WPP)

#### Overview

A sample application expanding on the `VME HME` sample, demonstrating how to performance motion estimation using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt) considering macro-block dependencies between macro-blocks. It introduces the concept of software scoreboard to handle dependencies between threads.

#### Help
To show help message run `launcher vme_wpp -- -h`.

#### Example
    launcher vme_wpp

### 5. VME Interlaced Processing

#### Overview

A sample application demonstrating the processing of interlaced context using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

#### Help
To show help message run `launcher vme_interlaced -- -h`.

#### Example
    launcher vme_interlaced

### 6. VME VAAPI Interoperability

#### Overview

A sample application demonstrating the processing of VAAPI created surfaces  using the Intel® OpenCL [`cl_intel_device_side_avc_motion_estimation` extension](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_device_side_avc_motion_estimation.txt).

This is only supported for Linux platforms for which VAAPI implementations exist. 

#### Help
To show help message run `launcher vme_interop -- -h`.

#### Example
    launcher vme_interop

_______
