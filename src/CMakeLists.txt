add_subdirectory(engine)

set(GAME_SOURCES
    main.cpp
    PlayerMovementSystem.cpp
)

add_executable(game ${GAME_SOURCES})
target_link_libraries(game PRIVATE ECS chipmunk)
