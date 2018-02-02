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
* VAAPI Interoperability
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
By default they are installed in `build` directory, but it can be customized by adding `CMAKE_INSTALL_PREFIX=your/path` during CMake configuration.

#### Windows
    Open compute_samples.sln
    Build INSTALL project

#### Linux
    make install

### Package
In order to create redistributable package please use the following commands:

#### Windows
    Open compute_samples.sln
    Build PACKAGE project

#### Linux
    make package

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
List of available samples and their overview can be found in [Samples Overview](docs/samples_overview.md).
