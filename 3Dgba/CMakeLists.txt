cmake_minimum_required(VERSION 3.25.1)

project(3Dgba VERSION 0.1.0 LANGUAGES ASM CXX C)

# Add the assembly files to the project
set(
    ASM_FILES
    asm/box/boxTranslate.s
)

set(
    SRC_FILES
    src/math/matrix.cpp
    src/math/sincos.cpp
    src/math/division_table_7_16.cpp
    src/math/division_table_14_32.cpp

    src/box.cpp
    src/rasterize.cpp
    src/ui_overlay.cpp
)

# Create the library target
add_library(3Dgba STATIC ${ASM_FILES} ${SRC_FILES})

# Loop through ASM_FILES and set source file properties to "ASM"
foreach(ASM_FILE ${ASM_FILES})
    set_source_files_properties(${ASM_FILE} PROPERTIES LANGUAGE "ASM")
endforeach()

target_include_directories(3Dgba PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS 3Dgba
    ARCHIVE DESTINATION lib/3Dgba
    LIBRARY DESTINATION lib/3Dgba
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/3Dgba
    DESTINATION include
)

option(WITH_UNIT_TESTS "Build gba_os/UnitTests.cpp" OFF)
if(WITH_UNIT_TESTS)
    add_subdirectory(test)
endif()