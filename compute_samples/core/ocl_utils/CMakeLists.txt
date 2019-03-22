#
# Copyright(c) 2018 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 3.8)
project(ocl_utils)

add_core_library(${PROJECT_NAME}
    SOURCE
    "include/ocl_utils/ocl_utils.hpp"
    "src/ocl_utils-impl.hpp"
    "src/ocl_utils.cpp"
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "src"
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    compute_samples::logging
    compute_samples::utils
    OpenCL::OpenCL
)

add_core_library_test(${PROJECT_NAME}
    SOURCE
    "test/main.cpp"
    "test/ocl_utils_unit_tests.cpp"
    "test/ocl_utils_integration_tests.cpp"
)
