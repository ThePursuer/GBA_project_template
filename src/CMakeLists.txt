# Add all .c files in the src directory to the executable target
file(GLOB_RECURSE SOURCES *.cpp **/*.cpp)

add_executable(my_gba_executable main.cpp ${SOURCES})

target_link_libraries(my_gba_executable PRIVATE gba mm ${CMAKE_SOURCE_DIR}/lib/liblibfixmath.a ${CMAKE_SOURCE_DIR}/lib/soundbank_bin.s ${CMAKE_SOURCE_DIR}/lib/wizard_spritesheet_calciumtrice_indexed.s ${CMAKE_SOURCE_DIR}/lib/shapes.s)
target_include_directories(my_gba_executable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
gba_create_rom(my_gba_executable)
