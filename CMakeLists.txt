# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)

# Set the CMake toolchain file
list(APPEND CMAKE_MODULE_PATH C:/devkitPro/cmake)
include(GBA)
include(Platform/NintendoGBA)

# Set the project name and version
project(my_gba_project VERSION 1.0)

add_subdirectory(src)