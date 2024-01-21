# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.25)
include(FetchContent)

# Set the project name and version
project(my_gba_project VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

# Set the CMake toolchain file
include($ENV{DEVKITPRO}/cmake/GBA.cmake)

FetchContent_Declare(
  libfixmath
  GIT_REPOSITORY https://github.com/PetteriAimonen/libfixmath
  GIT_TAG d308e466e1a09118d03f677c52e5fbf402f6fdd0
  PATCH_COMMAND git apply /home/peter/GBA_project_template/patches/libfixmath.patch
)
FetchContent_MakeAvailable(libfixmath)

target_compile_definitions(libfixmath PUBLIC
    FIXMATH_FAST_SIN
    FIXMATH_NO_64BIT
    # FIXMATH_NO_CACHE
    FIXMATH_NO_HARD_DIVISION
    # FIXMATH_NO_OVERFLOW
    # FIXMATH_NO_ROUNDING
    # FIXMATH_OPTIMIZE_8BIT
)

dkp_add_embedded_binary_library(meshes untitled.obj)

add_subdirectory(GBA_OS)
# add_subdirectory(ECS)
add_subdirectory(src)