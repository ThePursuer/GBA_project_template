# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.25)
include(FetchContent)

# Set the CMake toolchain file
include($ENV{DEVKITPRO}/cmake/GBA.cmake)
include($ENV{DEVKITPRO}/cmake/Platform/NintendoGBA.cmake)

# Set the project name and version
project(my_gba_project VERSION 1.0)


FetchContent_Declare(
  libfixmath
  URL      https://codeload.github.com/PetteriAimonen/libfixmath/zip/d308e466e1a09118d03f677c52e5fbf402f6fdd0
)
FetchContent_MakeAvailable(libfixmath)

add_subdirectory(src)