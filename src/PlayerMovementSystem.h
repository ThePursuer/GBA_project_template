#ifndef PLAYERMOVEMENTSYSTEM_H
#define PLAYERMOVEMENTSYSTEM_H

#include <bitset>

#include <gba_input.h>

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"
#include "engine/ECS/Systems/InputSystem.h"
#include "engine/ECS/Systems/AnimationSystem.h"
#include "engine/ECS/Systems/AudioSystem.h"

#include "Components.h"

enum PLAYER_ANIMATIONS: u8{
    IDLE = 1,
    WALK,
    FIRE,
    DIE
};

class PlayerInputStateComponent: public Component{
public:
    s8 deltaX, deltaY = 0;
    PLAYER_ANIMATIONS next_animation = PLAYER_ANIMATIONS::IDLE;
    bool loop_animation = false;
    u8 flipX = 0;
    std::bitset<4> changes = {};

    enum {
        DELTAX,
        DELTAY,
        NEXT_ANIMATION,
        FLIPX
    };
};

class PlayerMovementSystem: public ISystem{
public:
    PlayerMovementSystem(InputSystem& inputSystem, AudioSystem& audioSystem);
    void initialize(EntityManager& entityManager);
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void turnLeft();
    void turnRight();
    void setIdle();
    void fireWand();
    void die();

    Signal<mm_sfxhand&, mm_word> playFX;
    Signal<mm_sfxhand> stopFX;
private:
    Entity playerInputStateManager;
    std::shared_ptr<PlayerInputStateComponent> playerInputStateComponent;

    mm_sfxhand ambhand;
};

void registerInputs(InputSystem& inputHandler, PlayerMovementSystem& pms);
void registerSoundFX(AudioSystem& audioSystem, PlayerMovementSystem& pms);

#endif // PLAYERMOVEMENTSYSTEM_H