cmake_minimum_required(VERSION 2.8)

# Use this policy if exists
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

project(wzwin C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

add_subdirectory(src)
