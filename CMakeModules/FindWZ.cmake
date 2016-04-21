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
