cmake_minimum_required(VERSION 3.8)
project(compute_samples)

option(BUILD_BOOST "Download and build Boost" ON)
option(BUILD_PNG "Download and build libpng" ON)
option(DOWNLOAD_MEDIA "Download media files" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(THIRD_PARTY_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
set(MEDIA_ROOT_DIRECTORY ${CMAKE_SOURCE_DIR}/mediadata)
set(MEDIA_DIRECTORY ${MEDIA_ROOT_DIRECTORY}/merged)
set(BIN_DIRECTORY bin)

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
