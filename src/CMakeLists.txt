# Add all .c files in the src directory to the executable target
file(GLOB SOURCES *.c **/*.c)
add_executable(my_gba_executable ${SOURCES})

target_include_directories(my_gba_executable PRIVATE ${DEVKITPRO}/libgba/include) # Why doesn't this work???

# Set the linker directories for devkitPro
target_link_libraries(my_gba_executable PRIVATE ${DEVKITPRO}/libgba/lib/libgba.a)

# Compile with the appropriate flags for the GBA
target_compile_options(my_gba_executable PRIVATE
  -mthumb
  -mthumb-interwork
  -marm
  -mlong-calls
  -Wall
  -Wextra
  -pedantic
)

# Link the objects into an ELF file
target_link_options(my_gba_executable PRIVATE
  -specs=gba.specs
  -mthumb
  -mthumb-interwork
  -marm
  -mlong-calls
  -Wl,-Map=output.map
)

# Use objcopy to create the .gba file
add_custom_command(TARGET my_gba_executable POST_BUILD
  COMMAND ${DEVKITPRO}/devkitARM/bin/arm-none-eabi-objcopy.exe
          -O binary
          $<TARGET_FILE:my_gba_executable>
          ${CMAKE_SOURCE_DIR}/bin/my_gba_file.gba
)

set_property(
  TARGET my_gba_executable
  APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_SOURCE_DIR}/bin/my_gba_file.gba
)
