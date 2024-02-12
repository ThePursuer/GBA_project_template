# Set the source file
set(
    SRC_FILES
    test_main.cpp
    matrix_test.cpp
    rasterize_test.cpp
    math_test.cpp
)

add_executable(3Dgba_unit_test ${SRC_FILES})
target_link_libraries(3Dgba_unit_test PUBLIC 3Dgba gba_os)

gba_create_rom(3Dgba_unit_test)
dkp_target_generate_symbol_list(3Dgba_unit_test)