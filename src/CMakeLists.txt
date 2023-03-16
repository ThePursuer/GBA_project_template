# Add all .c files in the src directory to the executable target
file(GLOB_RECURSE SOURCES *.cpp **/*.cpp)

add_executable(my_gba_executable ${SOURCES})

target_link_libraries(my_gba_executable PRIVATE gba)
target_include_directories(my_gba_executable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
gba_create_rom(my_gba_executable)