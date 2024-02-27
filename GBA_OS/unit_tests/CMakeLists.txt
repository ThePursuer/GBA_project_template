# Set the source file
set(
    SRC_FILES
    test_main.cpp
    clock_test.cpp
)

add_executable(GBAOS_unit_test ${SRC_FILES})
target_link_libraries(GBAOS_unit_test PUBLIC gba_os)

gba_create_rom(GBAOS_unit_test)
# dkp_target_generate_symbol_list(GBAOS_unit_test)