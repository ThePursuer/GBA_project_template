#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <cassert>

#include "Sprites/wizard_spritesheet_calciumtrice_indexed.h"
// #include "Sprites/shapes.h"

#include "engine/Graphics/Sprite.h"
#include "engine/Input/InputHandler.h"
#include "engine/Math/Collider.h"
#include "engine/Math/ColliderI.h"

#include "engine/ECS/EngineComponents.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/EngineSystems.h"

// Input callbacks
// void moveUp() {
//     player_->setPosition(player_->getX(), player_->getY() - 1);
//     // player_->playAnimation(WALK, true);
// }
// void moveDown() {
//     player_->setPosition(player_->getX(), player_->getY() + 1);
//     // player_->playAnimation(WALK, true);
// }
// void moveLeft() {
//     player_->setPosition(player_->getX() - 1, player_->getY());
//     // player_->playAnimation(WALK, true);
// }
// void moveRight() {
//     player_->setPosition(player_->getX() + 1, player_->getY());
//     // player_->playAnimation(WALK, true);
// }
// void turnLeft(){
//     // player_sprite->flipX(1);
// }
// void turnRight(){
//     // player_sprite->flipX(0);
// }
// void setIdle(){
//     // player_->playAnimation(IDLE, true);
// }
// void fireWand(){
//     // player_->playAnimation(FIRE, false);
// }
// void die(){
//     // player_->playAnimation(DIE, false);
// }

// void registerInputs(InputHandler& inputHandler){
//     inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_UP, moveUp);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_DOWN, moveDown);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_LEFT, moveLeft);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_RIGHT, moveRight);
// 
//     inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_UP, setIdle);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_DOWN, setIdle);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_LEFT, setIdle);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_RIGHT, setIdle);
// 
//     inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_LEFT, turnLeft);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_RIGHT, turnRight);
// 
//     inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_A, fireWand);
//     inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_B, die);
// }

enum PLAYER_ANIMATIONS{
    IDLE,
    WALK,
    FIRE,
    DIE
};

std::shared_ptr<SpriteComponent> player_sprite;
Entity generatePlayer(EntityManager & manager){


    std::shared_ptr<AnimationComponent> player_animations = std::make_shared<AnimationComponent>();
    {
        std::vector<u32> idle;
        for(auto i = 0; i < 10; i++)
            idle.push_back(1 + i * 16);
        player_animations->sequences_.emplace(IDLE, Animation(std::move(idle), std::chrono::duration_cast<gba_microseconds>(gba_milliseconds(200))));

        //add walk anim
        std::vector<u32> walk;
        for(auto i = 20; i < 30; i++)
            walk.push_back(1 + i * 16);
        player_animations->sequences_.emplace(WALK, Animation(std::move(walk), std::chrono::duration_cast<gba_microseconds>(gba_milliseconds(200))));

        // add  fire animation
        std::vector<u32> fire;
        for(auto i = 30; i < 40; i++)
            fire.push_back(1 + i * 16);
        player_animations->sequences_.emplace(FIRE, Animation(std::move(fire), std::chrono::duration_cast<gba_microseconds>(gba_milliseconds(200))));

        // add  death animation
        std::vector<u32> death;
        for(auto i = 40; i < 50; i++)
            death.push_back(1 + i * 16);
        player_animations->sequences_.emplace(DIE, Animation(std::move(death), std::chrono::duration_cast<gba_microseconds>(gba_milliseconds(200))));
    }

    player_sprite = std::make_shared<SpriteComponent>();
    player_sprite->sprite = std::make_unique<Sprite>(ATTR0_COLOR_16 | ATTR0_NORMAL | ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PRIORITY(0) | ATTR2_PALETTE(0),
        1
    );

    std::shared_ptr<PositionComponent> player_position = std::make_shared<PositionComponent>();
    player_position->x = 80;
    player_position->y = 50;

    Entity player = manager.CreateEntity();

    manager.addComponent(player, EngineReservedComponents::ANIMATION, COMPONENT_CAST(player_animations));
    manager.addComponent(player, EngineReservedComponents::SPRITE, COMPONENT_CAST(player_sprite));
    manager.addComponent(player, EngineReservedComponents::POSITION, COMPONENT_CAST(player_position));

    return player;
}

int main() {
    // Set the timer frequency (16.38 kHz) and enable the timer
    REG_TM0CNT_H = TIMER_COUNT | (1 << 2) | TIMER_START;

    // Initialize GBA and devkitPro libraries
    irqInit();
    irqEnable(IRQ_VBLANK);

    // Set up the video mode and enable sprites
    SetMode(MODE_0 | OBJ_ENABLE | OBJ_1D_MAP);

    CpuFastSet(wizard_spritesheet_calciumtrice_indexedPal, SPRITE_PALETTE, wizard_spritesheet_calciumtrice_indexedPalLen / 4);
    CpuFastSet(wizard_spritesheet_calciumtrice_indexedTiles, SPRITE_GFX + 16, wizard_spritesheet_calciumtrice_indexedTilesLen / 4);

    std::set<ComponentType> indexableComponents = {EngineReservedComponents::ANIMATION, EngineReservedComponents::POSITION, EngineReservedComponents::SPRITE};
    EntityManager entityManager(indexableComponents);
    Entity player = generatePlayer(entityManager);

    AnimationSystem animationSystem;
    animationSystem.playAnimation(entityManager, player, DIE, true);

    auto clock = GbaClock::instance();

    // Initialize InputHandler
    InputHandler inputHandler;
    // registerInputs(inputHandler);

    auto last_frame_time = clock.now();
    // Main game loop
    while (1) {      
        auto now = clock.now();      
        animationSystem.update(entityManager, std::chrono::duration_cast<gba_microseconds>(now - last_frame_time));
        last_frame_time = now;
        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
