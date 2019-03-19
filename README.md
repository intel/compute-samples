# Intel® GPU Compute Samples
[![Build Status](https://travis-ci.org/intel/compute-samples.svg?branch=master)](https://travis-ci.org/intel/compute-samples)
[![Build status](https://ci.appveyor.com/api/projects/status/f3c7oe4i0eg1kjx7?svg=true)](https://ci.appveyor.com/project/intel/compute-samples)

Sample GPU Compute applications for Intel® Processor Graphics that demonstrate:

- notable new features in Compute APIs in released Graphics drivers
- features specific to Intel® Processor Graphics
- efficient way of using Intel® GPUs for general computing

Compute samples are currently limited to the OpenCL™ compute API.

## Samples Overview
List of all available samples and their description can be found in the [Samples Overview](docs/samples_overview.md).

## Getting Started

### Prerequisites
* [Intel® Graphics 5XX (Skylake) or newer](https://en.wikipedia.org/wiki/Intel_HD_and_Iris_Graphics)
* [OpenCL GPU driver](https://software.intel.com/en-us/articles/opencl-drivers)
* Compiler with C++11 support
  * [GCC 5.4](https://gcc.gnu.org/) or newer
  * [Microsoft Visual Studio 2015](https://www.visualstudio.com/) or newer
  * [Clang 3.8](https://clang.llvm.org/) or newer
* [CMake 3.8](https://cmake.org/) or newer
* VAAPI Interoperability on Linux
  * [libva, libva-drm, libva-x11 (VAAPI runtime libraries for Linux)](https://01.org/linuxgraphics/community/vaapi)
  * [libX11 (X Windows client library)](https://www.x.org)

### Build

    git clone https://github.com/intel/compute-samples
    cd compute-samples
    mkdir build
    cd build
    cmake ..
    cmake --build .

More details about building, installing and testing can be found in the [BUILD](BUILD.md) file.

### Testing
In order to run tests please read [this](BUILD.md#Test) section.

## Versioning
We use [Semantic Versioning](http://semver.org/). Current version is `0.y.z` which means that API may change without maintaining backward compatibility.

## Contributing
Please read [CONTRIBUTING](CONTRIBUTING.md) file for details about the contribution process.

## License
Intel® GPU Compute Samples are licensed under the [MIT License](LICENSE).
