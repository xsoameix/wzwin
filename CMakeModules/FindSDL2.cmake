# - Try to find the SDL2 libraries
#  Once done this will define
#
#  SDL2_FOUND       - system has SDL2
#  SDL2_INCLUDE_DIR - the SDL2 include directory
#  SDL2_LIBRARIES   - the libraries needed to use SDL2
#
#  Copyright (c) 2007 Daniel Gollub <gollub@b1-systems.de>
#  Copyright (c) 2007-2009 Bjoern Ricks  <bjoern.ricks@gmail.com>
#  Copyright (c) 2016 Lien Chiang  <xsoameix@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set(_SDL2_ROOT_HINTS ${SDL2_ROOT_DIR} ENV SDL2_ROOT_DIR)
  find_path(SDL2_INCLUDE_DIR
    NAMES SDL2/SDL.h
    HINTS ${_SDL2_ROOT_HINTS}
    PATH_SUFFIXES "include")
  find_library(_SDL2_LIBRARY
    NAMES SDL2
    HINTS ${_SDL2_ROOT_HINTS}
    PATH_SUFFIXES "lib")
  find_library(_SDL2_MAIN_LIBRARY
    NAMES SDL2main
    HINTS ${_SDL2_ROOT_HINTS}
    PATH_SUFFIXES "lib")
  set(_SDL2_CORE_LIBRARY user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)
  set(_SDL2_JOYSTICK_LIBRARY dinput8 dxguid)
  set(SDL2_LIBRARIES
    ${_SDL2_LIBRARY}
    ${_SDL2_MAIN_LIBRARY}
    ${_SDL2_CORE_LIBRARY}
    ${_SDL2_JOYSTICK_LIBRARY})
  find_package_handle_standard_args(SDL2 "Could NOT find SDL2, try to set the path to SDL2 root folder in the system variable SDL2_ROOT_DIR"
    SDL2_LIBRARIES SDL2_INCLUDE_DIR)
else()
  find_package(PkgConfig QUIET REQUIRED)
  pkg_search_module(SDL2 REQUIRED QUIET sdl2)
  find_path(SDL2_INCLUDE_DIR NAMES SDL.h HINTS ${SDL2_INCLUDE_DIRS})
  find_package(PackageHandleStandardArgs QUIET REQUIRED)
  find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_LIBRARIES SDL2_INCLUDE_DIR
    VERSION_VAR SDL2_VERSION)
endif()

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARIES)
