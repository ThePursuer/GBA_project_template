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
#include "engine/Math/Collider.h"
#include "engine/Math/ColliderI.h"

#include "engine/ECS/Components/EngineComponents.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Systems/AnimationSystem.h"
#include "engine/ECS/Systems/InputSystem.h"
#include "engine/ECS/Systems/SpriteSystem.h"

#include "engine/Signal.h"

#include "PlayerMovementSystem.h"


std::shared_ptr<SpriteComponent> player_sprite;
Entity generatePlayer(EntityManager & manager){
    std::shared_ptr<AnimationComponent> player_animations = std::make_shared<AnimationComponent>();
    {
        std::vector<u32> idle;
        for(auto i = 0; i < 10; i++)
            idle.push_back(1 + i * 16);
        player_animations->sequences_.emplace(IDLE, Animation(std::move(idle), gba_milliseconds(200)));

        //add walk anim
        std::vector<u32> walk;
        for(auto i = 20; i < 30; i++)
            walk.push_back(1 + i * 16);
        player_animations->sequences_.emplace(WALK, Animation(std::move(walk), gba_milliseconds(200)));

        // add  fire animation
        std::vector<u32> fire;
        for(auto i = 30; i < 40; i++)
            fire.push_back(1 + i * 16);
        player_animations->sequences_.emplace(FIRE, Animation(std::move(fire), gba_milliseconds(200)));

        // add  death animation
        std::vector<u32> death;
        for(auto i = 40; i < 50; i++)
            death.push_back(1 + i * 16);
        player_animations->sequences_.emplace(DIE, Animation(std::move(death), gba_milliseconds(200)));
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
    player_sprite->sprite->setPosition(80, 50);

    Entity player = manager.CreateEntity();

    manager.addComponent(player, EngineReservedComponents::ANIMATION, COMPONENT_CAST(player_animations));
    manager.addComponent(player, EngineReservedComponents::SPRITE, COMPONENT_CAST(player_sprite));
    manager.addComponent(player, EngineReservedComponents::POSITION, COMPONENT_CAST(player_position));
    manager.addComponent(player, GAME_COMPONENTS::PLAYER_COMPONENT, COMPONENT_CAST(std::make_shared<PlayerComponent>()));

    return player;
}

int main() {
    // Initialize GBA and devkitPro libraries
    irqInit();
    irqEnable(IRQ_VBLANK);

    // Set up the video mode and enable sprites
    SetMode(MODE_0 | OBJ_ENABLE | OBJ_1D_MAP);


    CpuFastSet(wizard_spritesheet_calciumtrice_indexedPal, SPRITE_PALETTE, wizard_spritesheet_calciumtrice_indexedPalLen / 4);
    CpuFastSet(wizard_spritesheet_calciumtrice_indexedTiles, SPRITE_GFX + 16, wizard_spritesheet_calciumtrice_indexedTilesLen / 4);

    std::set<ComponentType> indexableComponents = {EngineReservedComponents::ANIMATION, EngineReservedComponents::POSITION, EngineReservedComponents::SPRITE, GAME_COMPONENTS::PLAYER_COMPONENT, GAME_COMPONENTS::PLAYER_INPUT_STATE_COMPONENT};
    EntityManager entityManager(indexableComponents);
    Entity player = generatePlayer(entityManager);

    AnimationSystem animationSystem;
    SpriteSystem spriteSystem;
    InputSystem inputSystem;
    PlayerMovementSystem movementSystem;
    movementSystem.initialize(entityManager);

    AnimationSystem::playAnimation(entityManager, player, PLAYER_ANIMATIONS::IDLE, true);

    registerInputs(inputSystem, movementSystem);
    auto clock = GbaClock::instance();
    auto last_frame_time = clock.now();
    // Main game loop
    unsigned int count = 0;
    gba_milliseconds accumulatedTime(0);
    while (1) {      
        count++;
        auto now = clock.now();
        inputSystem.update(entityManager, std::chrono::duration_cast<gba_microseconds>(now - last_frame_time));
        movementSystem.update(entityManager, std::chrono::duration_cast<gba_microseconds>(now - last_frame_time));
        animationSystem.update(entityManager, std::chrono::duration_cast<gba_microseconds>(now - last_frame_time));
        spriteSystem.update(entityManager, std::chrono::duration_cast<gba_microseconds>(now - last_frame_time));

        last_frame_time = now;
        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
