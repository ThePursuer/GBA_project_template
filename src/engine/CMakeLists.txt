set(
    ECS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/SpriteManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Clock/GbaClock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/EntityManger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/AnimationSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/AudioSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/InputSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/PhysicsSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/PositionSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS/Systems/SpriteSystem.cpp
)

add_library(ECS STATIC ${ECS_SOURCES})
target_include_directories(ECS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
