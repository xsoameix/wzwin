# - Try to find the WZ libraries
#  Once done this will define
#
#  WZ_FOUND       - system has WZ
#  WZ_INCLUDE_DIR - the WZ include directory
#  WZ_LIBRARIES   - the libraries needed to use WZ
#
#  Copyright (c) 2016 Lien Chiang  <xsoameix@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set(_WZ_ROOT_HINTS ${WZ_ROOT_DIR} ENV WZ_ROOT_DIR)
  find_path(WZ_INCLUDE_DIR
    NAMES wz/file.h
    HINTS ${_WZ_ROOT_HINTS}
    PATH_SUFFIXES "include")
  find_library(_WZ_LIBRARY
    NAMES wz
    HINTS ${_WZ_ROOT_HINTS}
    PATH_SUFFIXES "lib")
  set(WZ_LIBRARIES ${_WZ_LIBRARY})
  find_package_handle_standard_args(WZ "Could NOT find WZ, try to set the path to WZ root folder in the system variable WZ_ROOT_DIR"
    WZ_LIBRARIES WZ_INCLUDE_DIR)
else()
  find_package(PkgConfig QUIET REQUIRED)
  pkg_search_module(WZ REQUIRED QUIET wz)
  find_path(WZ_INCLUDE_DIR NAMES wz/file.h HINTS ${WZ_INCLUDE_DIRS})
  find_package(PackageHandleStandardArgs QUIET REQUIRED)
  find_package_handle_standard_args(WZ
    REQUIRED_VARS WZ_LIBRARIES WZ_INCLUDE_DIR
    VERSION_VAR WZ_VERSION)
endif()

mark_as_advanced(WZ_INCLUDE_DIR WZ_LIBRARIES)
