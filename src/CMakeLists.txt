# Add all .c files in the src directory to the executable target
file(GLOB SOURCES *.cpp **/*.cpp)
add_executable(my_gba_executable ${SOURCES})

# dkp_add_embedded_binary_library(splash_lib ${SPLASH})
dkp_add_embedded_binary_library(bgf_target ${PROJECT_SOURCE_DIR}/data/tune.bgf)
dkp_add_embedded_binary_library(pcx_target ${PROJECT_SOURCE_DIR}/data/ScoutSplash.pcx)
# Set the linker directories for devkitPro
target_link_libraries(my_gba_executable PUBLIC mm gba pcx_target bgf_target)

gba_create_rom(my_gba_executable)