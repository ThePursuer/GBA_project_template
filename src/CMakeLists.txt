add_executable(my_gba_executable 
    main.cpp
)

target_link_libraries(my_gba_executable PUBLIC meshes gba gba_os libfixmath)

gba_create_rom(my_gba_executable)
dkp_target_generate_symbol_list(my_gba_executable)
