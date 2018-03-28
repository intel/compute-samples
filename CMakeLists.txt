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
project(compute_samples)

option(BUILD_BOOST "Download and build Boost" ON)
option(BUILD_PNG "Download and build libpng" ON)
option(DOWNLOAD_MEDIA "Download media files" ON)

set(COMPUTE_SAMPLES_MAJOR_VERSION 0)
set(COMPUTE_SAMPLES_MINOR_VERSION 1)
set(COMPUTE_SAMPLES_PATCH_VERSION 0)
execute_process(COMMAND git rev-parse --short HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE COMPUTE_SAMPLES_GIT_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT DEFINED COMPUTE_SAMPLES_BUILD_NUMBER)
    set(COMPUTE_SAMPLES_BUILD_NUMBER "dev")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THIRD_PARTY_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
set(MEDIA_ROOT_DIRECTORY ${CMAKE_SOURCE_DIR}/mediadata)
set(MEDIA_DIRECTORY ${MEDIA_ROOT_DIRECTORY}/merged)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Default installation directory" FORCE)
endif()

# Prepare mediadata
if(DOWNLOAD_MEDIA)
    set(MEDIADATA_ROOT ${MEDIA_ROOT_DIRECTORY}/external)
    configure_file(cmake/MediaData.CMakeLists.txt.in ${MEDIA_ROOT_DIRECTORY}/downloads/mediadata-external/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${MEDIA_ROOT_DIRECTORY}/downloads/mediadata-external)
    execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${MEDIA_ROOT_DIRECTORY}/downloads/mediadata-external)
endif()
file(COPY ${MEDIA_ROOT_DIRECTORY}/external/yuv DESTINATION ${MEDIA_DIRECTORY})
file(COPY ${MEDIA_ROOT_DIRECTORY}/internal/yuv DESTINATION ${MEDIA_DIRECTORY})
file(COPY ${MEDIA_ROOT_DIRECTORY}/internal/bmp DESTINATION ${MEDIA_DIRECTORY})
file(COPY ${MEDIA_ROOT_DIRECTORY}/internal/png DESTINATION ${MEDIA_DIRECTORY})

# Prepare Boost
if(BUILD_BOOST)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ADDRESS_MODEL 64)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ADDRESS_MODEL 32)
    endif()
    if(WIN32)
        set(BOOTSTRAP_BINARY bootstrap.bat)
        set(B2_BINARY b2.exe)
    elseif(UNIX)
        set(BOOTSTRAP_BINARY ./bootstrap.sh)
        set(B2_BINARY ./b2)
    endif()
    if(MSVC)
        if(${MSVC_VERSION} EQUAL 1900)
            set(TOOLSET "--toolset=msvc-14.0")
        elseif(${MSVC_VERSION} EQUAL 1910)
            set(TOOLSET "--toolset=msvc-14.1")
        endif()
    endif()
    set(BOOST_ROOT ${THIRD_PARTY_DIRECTORY}/boost)
    configure_file(cmake/Boost.CMakeLists.txt.in ${THIRD_PARTY_DIRECTORY}/downloads/boost-external/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/boost-external)
    execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/boost-external)
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.64 REQUIRED COMPONENTS log program_options timer chrono system)

# Prepare libpng
if(BUILD_PNG)
    set(ZLIB_ROOT ${THIRD_PARTY_DIRECTORY}/zlib)
    configure_file(cmake/ZLIB.CMakeLists.txt.in ${THIRD_PARTY_DIRECTORY}/downloads/zlib-external/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/zlib-external)
    execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/zlib-external)
    find_library(ZLIB_LIBRARY NAMES libz.a zlibstatic zlibstaticd PATHS ${ZLIB_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_path(ZLIB_INCLUDE_DIR NAMES zlib.h PATHS ${ZLIB_ROOT} PATH_SUFFIXES include)

    set(PNG_ROOT ${THIRD_PARTY_DIRECTORY}/libpng)
    configure_file(cmake/PNG.CMakeLists.txt.in ${THIRD_PARTY_DIRECTORY}/downloads/libpng-external/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/libpng-external)
    execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/libpng-external)
    find_library(PNG_LIBRARY NAMES libpng.a libpng16_static libpng16_staticd PATHS ${PNG_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    find_path(PNG_INCLUDE_DIR NAMES png.h PATHS ${PNG_ROOT} PATH_SUFFIXES include)

    set(CMAKE_PREFIX_PATH ${PNG_ROOT})
endif()

find_package(PNG REQUIRED)

# Prepare googletest
set(GTEST_ROOT ${THIRD_PARTY_DIRECTORY}/googletest)
configure_file(cmake/GTEST.CMakeLists.txt.in ${THIRD_PARTY_DIRECTORY}/downloads/googletest-external/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/googletest-external)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/googletest-external)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${GTEST_ROOT}/googletest-src"
                 "${GTEST_ROOT}/googletest-build" EXCLUDE_FROM_ALL)
add_library(GTest::GTest ALIAS gtest)
add_library(GTest::Main ALIAS gtest_main)
set_target_properties(gtest PROPERTIES FOLDER third_party/gtest)
set_target_properties(gtest_main PROPERTIES FOLDER third_party/gtest)

# Prepare OpenCL
set(OPENCL_ROOT ${THIRD_PARTY_DIRECTORY}/opencl)
configure_file(cmake/OPENCL.CMakeLists.txt.in ${THIRD_PARTY_DIRECTORY}/downloads/opencl-external/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/opencl-external)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/downloads/opencl-external)

set(CMAKE_PREFIX_PATH ${OPENCL_ROOT})
find_package(OpenCL REQUIRED)

enable_testing()

# Prepare project
add_subdirectory(compute_samples)

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_NAME "compute-samples")
set(CPACK_PACKAGE_VERSION "${COMPUTE_SAMPLES_MAJOR_VERSION}.${COMPUTE_SAMPLES_MINOR_VERSION}.${COMPUTE_SAMPLES_PATCH_VERSION}+${COMPUTE_SAMPLES_BUILD_NUMBER}.${COMPUTE_SAMPLES_GIT_COMMIT}")
include(CPack)
