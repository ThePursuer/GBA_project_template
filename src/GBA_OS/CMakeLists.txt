add_library( gba_os
    Core.cpp
    Screen.cpp
    Tasks.cpp
    Clock/GbaClock.cpp
)

target_link_libraries(gba_os PUBLIC gba)
target_compile_options(gba_os PRIVATE -Wno-volatile)