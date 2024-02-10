project(GBA_tools)


if(NOT DEFINED MAGIC_NUMBER_SOURCE_DIR)
set(MAGIC_NUMBER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_executable(magic_number_tester ${MAGIC_NUMBER_SOURCE_DIR}/division_table.cpp magic_number_test_app.cpp)
target_include_directories(magic_number_tester PRIVATE ${MAGIC_NUMBER_SOURCE_DIR})
