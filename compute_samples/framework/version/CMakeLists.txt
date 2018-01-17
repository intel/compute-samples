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
project(version)

set(COMPUTE_SAMPLES_MAJOR_VERSION 0)
set(COMPUTE_SAMPLES_MINOR_VERSION 1)
set(COMPUTE_SAMPLES_PATCH_VERSION 0)
execute_process(COMMAND git rev-parse --short HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE COMPUTE_SAMPLES_BUILD OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file("${PROJECT_SOURCE_DIR}/src/version_config.cpp.in" "${PROJECT_BINARY_DIR}/generated/version_config.cpp")

set(SOURCES
    include/version/version.hpp
    src/version.cpp
    src/version_config.hpp
    ${PROJECT_BINARY_DIR}/generated/version_config.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER framework/${PROJECT_NAME})
