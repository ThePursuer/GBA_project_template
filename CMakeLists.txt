# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.25.1)
include(FetchContent)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set the project name and version
project(my_gba_project VERSION 1.0 LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

# dkp_add_embedded_binary_library(meshes cube.obj cube.mtl)

# Set WITH_UNIT_TESTS option for GBA_OS subproject
# set(WITH_UNIT_TESTS ON CACHE BOOL "Enable unit tests for GBA_OS" FORCE)
add_subdirectory(GBA_OS)
# add_subdirectory(ECS)
add_subdirectory(src)
add_subdirectory(3Dgba)
