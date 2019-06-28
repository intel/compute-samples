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

add_application_library(subgroups_imagecopy
    SOURCE
    "include/subgroups_imagecopy/subgroups_imagecopy.hpp"
    "src/subgroups_imagecopy.cpp"
)
target_link_libraries(subgroups_imagecopy_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::image
    compute_samples::ocl_utils
)
add_kernels(subgroups_imagecopy_lib
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)

add_application(subgroups_imagecopy
    SOURCE
    "src/main.cpp"
)
install_kernels(subgroups_imagecopy
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)
install_resources(subgroups_imagecopy FILES "${MEDIA_DIRECTORY}/bmp/input.bmp")

add_application_test(subgroups_imagecopy
    SOURCE
    "test/main.cpp"
    "test/subgroups_imagecopy_integration_tests.cpp"
    "test/subgroups_imagecopy_system_tests.cpp"
)
install_kernels(subgroups_imagecopy_tests
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)
install_resources(subgroups_imagecopy_tests FILES "${MEDIA_DIRECTORY}/bmp/input.bmp")
