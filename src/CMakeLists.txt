add_executable(my_gba_executable main.cpp)

target_link_libraries(my_gba_executable PRIVATE gba 
    libfixmath
)

gba_create_rom(my_gba_executable)
