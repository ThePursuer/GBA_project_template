cmake_minimum_required(VERSION 3.25.1)

project(3Dgba VERSION 0.1.0 LANGUAGES ASM CXX C)

# Add the assembly files to the project
set(
    ASM_FILES
    asm/fixed_point/fix14_mul.s
    asm/fixed_point/sdiv32.s
    asm/fixed_point/udiv32.s

    asm/box/boxTranslate.s
)

set(
    SRC_FILES
    src/matrix.cpp
    src/sincos.cpp
    src/fixdivtable.cpp
    src/box.cpp
    src/rasterize.cpp
    # src/transform.cpp
)

# Create the library target
add_library(3Dgba STATIC ${ASM_FILES} ${SRC_FILES})
target_link_libraries(3Dgba PUBLIC gba_os)

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