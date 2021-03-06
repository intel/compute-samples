# Dependencies
The following packages are required to build the project:
* [Boost 1.71](http://www.boost.org/)
* [libpng 1.6.37](http://www.libpng.org/pub/png/libpng.html)
* [Google Test 1.10.0](https://github.com/google/googletest)
* VAAPI Interoperability on Linux
  * [libva, libva-drm, libva-x11 (VAAPI runtime libraries for Linux)](https://01.org/linuxgraphics/community/vaapi)
  * [libX11 (X Windows client library)](https://www.x.org)

There are utility scripts available that help with installation of all dependencies on Windows and Linux. Examples below:

    # Install dependencies for Windows
    scripts/install/install_windows.ps1

    # Install dependencies for Ubuntu 18.04
    scripts/install/install_ubuntu_18_04.sh

Note: By default all dependencies on Windows are built for x64.

# Build
Given that all dependencies are installed then use the following commands to build the project:

    mkdir build
    cd build
    cmake ..
    # cmake .. -A x64 on Visual Studio, because by default solutions are generated for x86
    cmake --build .

In order to use a different generator than a default one please use `-G` option based on the [CMake documentation](https://cmake.org/cmake/help/v3.8/manual/cmake-generators.7.html). 
For example, you can generate a Visual Studio 2017 solution by running: `cmake .. -G "Visual Studio 15 2017 Win64"`

## Build samples for specific API
Our project currently supports OpenCL and oneApi Level Zero APIs. By default all samples are enabled and built.
If necessary you can disable specific API by using `WITH_OCL=OFF` or `WITH_L0=OFF` CMake flags.

For example:

    cmake .. -DWITH_L0=OFF
    cmake --build .

# Installation
The following command installs project's binaries.
By default they are installed in the `build` directory, but it can be customized by adding `CMAKE_INSTALL_PREFIX=your/path` during CMake configuration.

    cmake --build . --target install

# Package
In order to create a redistributable package please use the following command:

    cmake --build . --target package

# Source Package
In order to create a redistributable package of the source please use the following command:

    cmake --build . --target package_source

# CMake options
CMake can be configured using the following parameters:

* `BOOST_ROOT` - Path to the custom Boost installation directory.
* `PNG_ROOT` - Path to the custom libpng installation directory.
* `GTEST_ROOT` - Path to the custom Google Test installation directory.
* `ZLIB_ROOT` - Path to the custom zlib installation directory.
* `OPENCL_ROOT` - Path to the custom OpenCL ICD installation directory.
* `MEDIADATA_ROOT` - Path to the custom mediadata directory.
* `L0_ROOT` - Path to the custom Level Zero ICD installation directory.
* `VA_ROOT` - Path to the custom libva installation directory.
* `WITH_OCL` - If set to `OFF` disables building the OCL samples and utilities. Default is `ON`.
* `WITH_L0` - If set to `OFF` disables building the L0 samples. Default is `ON`.

Example command line with custom options: `cmake .. -DBOOST_ROOT=/home/boost_1_64_0`

# Test
There are 2 ways to run tests depending on the use case:

1. Using [CTest](https://cmake.org/cmake/help/v3.8/manual/ctest.1.html):
    * To run all registered tests from all modules run `ctest` from the `build` directory.
    * To run all tests requiring specific HW run `ctest -R .*hw`.
    * To run all tests not requiring specific HW run `ctest -E .*hw`.

    If you want to run `ctest` on a different configuration like `Debug` or `Release` please use `-C` option according to the [CTest documentation](https://cmake.org/cmake/help/v3.8/manual/ctest.1.html).

1. Using [Google Test](https://github.com/google/googletest/tree/release-1.10.0):
    * To run all tests from a specific module find module's tester and run it.
    * To run specific tests from a specific module find module's tester and run it with `--gtest_filter` option.

Tests that require specific hardware to run should be marked as `HWTEST` using defines available in [test_harness](./compute_samples/core/test_harness/include/test_harness/test_harness.hpp).
