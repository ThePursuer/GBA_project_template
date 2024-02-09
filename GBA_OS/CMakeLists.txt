# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.25)

# Set the project name and version
project(gba_os VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

# Set the CMake toolchain file
include($ENV{DEVKITPRO}/cmake/GBA.cmake)

set(GBA_OS_SRC
    gba_os/Console.cpp
    gba_os/Core.cpp
    gba_os/GbaClock.cpp
    gba_os/OsErrors.cpp
    gba_os/Screen.cpp
    gba_os/Tasks.cpp
)

set(GBA_OS_LINK_LIBS
    gba
)

option(BUILD_WITH_MAXMOD "Build the library with Maxmod support" ON)
if(BUILD_WITH_MAXMOD)
    set(GBA_OS_SRC ${GBA_OS_SRC} gba_os/MaxMod.cpp)
    set(GBA_OS_LINK_LIBS ${GBA_OS_LINK_LIBS} mm)
endif()

option(WITH_UNIT_TESTS "Build gba_os/UnitTests.cpp" OFF)
if(WITH_UNIT_TESTS)
    set(GBA_OS_SRC ${GBA_OS_SRC} gba_os/UnitTests.cpp)
endif()

add_library(gba_os STATIC ${GBA_OS_SRC})

target_link_libraries(gba_os PUBLIC ${GBA_OS_LINK_LIBS})
target_compile_options(gba_os PRIVATE -Wno-volatile)
dkp_target_generate_symbol_list(gba_os)

target_include_directories(gba_os PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS gba_os
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)
