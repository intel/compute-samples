# Dependencies
The following packages are required to build the project:
* [Boost 1.64](http://www.boost.org/)
* [libpng 1.6.30](http://www.libpng.org/pub/png/libpng.html)
* [Google Test 1.8.1](https://github.com/google/googletest)

There are utility scripts available that help with installation of all dependencies on Windows and Linux. Examples below:

    # Install dependencies for Visual Studio 2017
    scripts/install/install_vs2017.ps1

    # Install dependencies for Ubuntu 18.04
    scripts/install/install_ubuntu_18_04.sh

Note: In order to build the project using GCC 8, Boost 1.68 is required, becasue of [this](https://github.com/boostorg/compute/issues/778) bug in Boost::Compute.

# Build
Given that all dependencies are installed then use the following commands to build the project:

    mkdir build
    cd build
    cmake ..
    cmake --build .

In order to use a different generator than a default one please use `-G` option based on the [CMake documentation](https://cmake.org/cmake/help/v3.8/manual/cmake-generators.7.html). 
For example, you can generate a Visual Studio 2017 solution by running: `cmake .. -G "Visual Studio 15 2017"`

# Installation
The following command installs project's binaries.
By default they are installed in the `build` directory, but it can be customized by adding `CMAKE_INSTALL_PREFIX=your/path` during CMake configuration.

    cmake --build . --target install

# Package
In order to create a redistributable package please use the following command:

    cmake --build . --target package

# Source Package
In order to create a redistributable source package please use the following command:

    cmake --build . --target package_source

# CMake options
CMake can be configured using the following parameters:

* `BOOST_ROOT` - Path to the custom Boost installation directory.
* `PNG_ROOT` - Path to the custom libpng installation directory.
* `GTEST_ROOT` - Path to the custom Google Test installation directory.
* `ZLIB_ROOT` - Path to the custom zlib installation directory.
* `OPENCL_ROOT` - Path to the custom OpenCL ICD installation directory.
* `MEDIADATA_ROOT` - Path to the custom mediadata directory.

Example command line with custom options: `cmake .. -DBOOST_ROOT=/home/boost_1_64_0`

# Test
There are 3 ways to run tests depending on the use case:

* To run all registered tests from all modules run `ctest` from the `build` directory.
* To run all tests from a specific module find module's tester and run it.
* To run specific tests from a specific module find module's tester and run it with `--gtest_filter` option or use the `ctest -R *test_pattern*` option.

If you want to run `ctest` on a different configuration like `Debug` or `Release` please use `-C` option according to the [CTest documentation](https://cmake.org/cmake/help/v3.8/manual/ctest.1.html).
