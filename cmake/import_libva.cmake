#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

if(NOT TARGET VA::VA)
    find_path(VA_INCLUDE_DIR
      NAMES va/va.h
      PATHS
      ${VA_ROOT}
      PATH_SUFFIXES "include"
    )

    find_library(VA_LIBRARY
      NAMES va
      PATHS
      ${VA_ROOT}
      PATH_SUFFIXES "lib"
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(VA
      REQUIRED_VARS
        VA_INCLUDE_DIR
        VA_LIBRARY
      HANDLE_COMPONENTS
    )
    mark_as_advanced(VA_LIBRARY VA_INCLUDE_DIR)

    set(VA_LIBRARIES ${VA_LIBRARY})
    set(VA_INCLUDE_DIRS ${VA_INCLUDE_DIR})

    add_library(VA::VA INTERFACE IMPORTED)
    set_target_properties(VA::VA
      PROPERTIES INTERFACE_LINK_LIBRARIES "${VA_LIBRARIES}"
    )
    set_target_properties(VA::VA
      PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VA_INCLUDE_DIRS}"
    )
endif()
