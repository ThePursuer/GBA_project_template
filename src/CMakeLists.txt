add_executable(my_gba_executable 
    main.cpp
    GBA_OS/Clock/GbaClock.cpp
)

# # Generate assembly files from source files
# foreach(source_file main.cpp GBA_OS/Clock/GbaClock.cpp)
#     get_filename_component(source_name ${source_file} NAME_WE)
#     set(asm_output "${CMAKE_BINARY_DIR}/${source_name}.s")
#     add_custom_command(
#         OUTPUT ${asm_output}
#         COMMAND ${CMAKE_CXX_COMPILER} -I ${CMAKE_SOURCE_DIR}/src/GBA_OS/Clock/ -I /opt/devkitpro/libgba/include -I ${CMAKE_SOURCE_DIR}/build/_deps/libfixmath-src -S ${CMAKE_CURRENT_SOURCE_DIR}/${source_file} -o ${asm_output}
#         DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${source_file}
#         COMMENT "Generating assembly from ${source_file}"
#     )
#     list(APPEND asm_files ${asm_output})
# endforeach()

# add_custom_target(generate_asm_files DEPENDS ${asm_files})

target_link_libraries(my_gba_executable PUBLIC gba libfixmath)

gba_create_rom(my_gba_executable)

# Make sure the assembly generation is executed before building the executable
# add_dependencies(my_gba_executable generate_asm_files)
