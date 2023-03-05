# Add all .c files in the src directory to the executable target
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
set(TARGET_NAME my_gba_program.gba)
add_executable(${TARGET_NAME} ${SOURCES})

# Add the GBA include directory and library directory to the include path
target_include_directories(${TARGET_NAME} PRIVATE
  "${DEVKITPRO_PATH}/libgba/include"
  "${DEVKITPRO_PATH}/libgba/lib"
)

# Set the linker directories for devkitPro
target_link_libraries(${TARGET_NAME} PRIVATE gba)

# Compile with the appropriate flags for the GBA
target_compile_options(${TARGET_NAME} PRIVATE
  -mthumb
  -mthumb-interwork
  -marm
  -mlong-calls
  -Wall
  -Wextra
  -pedantic
)

# Link the objects into an ELF file
target_link_options(${TARGET_NAME} PRIVATE
  -specs=gba.specs
  -mthumb
  -mthumb-interwork
  -marm
  -mlong-calls
  -Wl,-Map=output.map
)

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Use objcopy to create the .gba file
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${DEVKITPRO_PATH}/devkitARM/bin/arm-none-eabi-objcopy.exe
          -O binary
          $<TARGET_FILE:${TARGET_NAME}>
          my_gba_file.gba
)