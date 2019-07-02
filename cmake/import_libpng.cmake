#
# Copyright(c) 2019 Intel Corporation
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

if(NOT TARGET PNG::PNG)
    # Find static zlib
    find_library(ZLIB_LIBRARY NAMES libz.a zlibstatic zlibstaticd PATHS "${ZLIB_ROOT}" PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_path(ZLIB_INCLUDE_DIR NAMES zlib.h PATHS "${ZLIB_ROOT}" PATH_SUFFIXES include NO_DEFAULT_PATH)

    # Find static libpng
    find_library(PNG_LIBRARY NAMES libpng.a libpng16_static libpng16_staticd PATHS "${PNG_ROOT}" PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_path(PNG_INCLUDE_DIR NAMES png.h PATHS "${PNG_ROOT}" PATH_SUFFIXES include NO_DEFAULT_PATH)

    set(CMAKE_PREFIX_PATH "${PNG_ROOT}")
    find_package(PNG REQUIRED)
endif()
