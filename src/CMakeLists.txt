# Add all .c files in the src directory to the executable target
file(GLOB SOURCES *.cpp **/*.cpp)
add_executable(my_gba_executable ${SOURCES})

# Set the linker directories for devkitPro
target_link_libraries(my_gba_executable PRIVATE gba)
gba_create_rom(my_gba_executable)