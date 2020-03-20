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
project(compute_samples VERSION 0.3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(compile_flags)
include(custom_functions)
include(default_paths)
include(set_version)

include(import_boost)
include(import_gtest)
include(import_libpng)
include(import_mediadata)

option(WITH_OCL "Build OCL Samples" ON)
if(WITH_OCL)
    include(import_opencl)
endif()

option(WITH_L0 "Build L0 Samples" ON)
if(WIN32)
    set(WITH_L0 OFF)
endif()

if(WITH_L0)
    include(import_level_zero)
endif()

enable_testing()
add_subdirectory(${PROJECT_NAME})

list(APPEND IGNORE_SAMPLE_SOURCE "/\.git/")
list(APPEND IGNORE_SAMPLE_SOURCE ".travis.yml")
list(APPEND IGNORE_SAMPLE_SOURCE "appveyor.yml")
list(APPEND IGNORE_SAMPLE_SOURCE "shippable.yml")
list(APPEND IGNORE_SAMPLE_SOURCE ".gitignore")
list(APPEND IGNORE_SAMPLE_SOURCE "/docker/")
list(APPEND IGNORE_SAMPLE_SOURCE "/build/")

generate_packages(${PROJECT_NAME} IGNORE_FILES ${IGNORE_SAMPLE_SOURCE})
