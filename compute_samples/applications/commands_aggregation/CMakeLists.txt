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
project(commands_aggregation)

add_application_library(${PROJECT_NAME}
    SOURCE
    "include/commands_aggregation/commands_aggregation.hpp"
    "src/commands_aggregation.cpp"
)
target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
    compute_samples::timer
    compute_samples::ocl_utils
    Boost::log
    Boost::program_options
)
add_kernels(${PROJECT_NAME}_lib "commands_aggregation.cl")

add_application(${PROJECT_NAME}
    SOURCE
    "src/main.cpp"
)
install_kernels(${PROJECT_NAME} "commands_aggregation.cl")

add_application_test(${PROJECT_NAME}
    SOURCE
    "test/commands_aggregation_integration_tests.cpp"
    "test/commands_aggregation_system_tests.cpp"
)
install_kernels(${PROJECT_NAME}_tests "commands_aggregation.cl")
