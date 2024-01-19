add_library( ECS STATIC
    src/EntityManager.cpp
    src/SystemManger.cpp
    src/Systems/AudioSystem.cpp
    src/Systems/InputSystem.cpp
)

target_link_libraries(ECS PUBLIC gba_os mm)
target_include_directories(ECS PUBLIC include)

target_include_directories(ECS PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>
)

install(TARGETS ECS
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)
