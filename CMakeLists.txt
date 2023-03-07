# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the CMake toolchain file
set(CMAKE_TOOLCHAIN_FILE C:/devkitPro/cmake/GBA.cmake)

# Set the project name and version
project(my_gba_project VERSION 1.0)


add_subdirectory(src)