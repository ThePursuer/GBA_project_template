# Add all .c files in the src directory to the executable target
file(GLOB_RECURSE SOURCES *.cpp **/*.cpp)

add_executable(my_gba_executable main.cpp ${SOURCES})

target_link_libraries(my_gba_executable PRIVATE 
    gba 
    mm
    ${CMAKE_SOURCE_DIR}/assets/audio/build/.dkp-generated/soundbank_bin/soundbank_bin.s 
    ${CMAKE_SOURCE_DIR}/assets/sprites/wizard_spritesheet_calciumtrice_indexed.s 
    ${CMAKE_SOURCE_DIR}/assets/sprites/shapes.s 
    chipmunk
)
target_include_directories(my_gba_executable PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/assets/sprites 
    ${CMAKE_SOURCE_DIR}/assets/audio 
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/assets/audio/build/.dkp-generated/soundbank_bin
    /c/Users/peter/Development/Chipmunk2D/include
)
gba_create_rom(my_gba_executable)
dkp_target_generate_symbol_list(my_gba_executable)
