# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the path to the devkitPro toolchain
set(DEVKITPRO_PATH C:/devkitPro)
# Set the C and C++ compilers to those in the devkitPro toolchain
set(CMAKE_C_COMPILER ${DEVKITPRO_PATH}/devkitARM/bin/arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER ${DEVKITPRO_PATH}/devkitARM/bin/arm-none-eabi-g++.exe)

# Set the project name and version
project(my_gba_project VERSION 1.0)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build")
link_directories("${DEVKITPRO_PATH}/libgba/lib")

add_subdirectory(src)