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

set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
set(MEDIA_ROOT_DIRECTORY "${CMAKE_SOURCE_DIR}/mediadata")
set(MEDIA_DIRECTORY "${MEDIA_ROOT_DIRECTORY}/merged")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Default installation directory" FORCE)
endif()

if(NOT DEFINED BOOST_ROOT)
    set(BOOST_ROOT "${THIRD_PARTY_DIRECTORY}/boost")
endif()

if(NOT DEFINED GTEST_ROOT)
    set(GTEST_ROOT "${THIRD_PARTY_DIRECTORY}/googletest")
endif()

if(NOT DEFINED ZLIB_ROOT)
    set(ZLIB_ROOT "${THIRD_PARTY_DIRECTORY}/zlib")
endif()

if(NOT DEFINED PNG_ROOT)
    set(PNG_ROOT "${THIRD_PARTY_DIRECTORY}/libpng")
endif()

if(NOT DEFINED MEDIADATA_ROOT)
    set(MEDIADATA_ROOT "${MEDIA_ROOT_DIRECTORY}/external")
endif()

if(NOT DEFINED OPENCL_ROOT)
    set(OPENCL_ROOT "${THIRD_PARTY_DIRECTORY}/opencl")
endif()
