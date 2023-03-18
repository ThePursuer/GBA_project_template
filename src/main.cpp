#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>

#include "Sprites/wizard_spritesheet_calciumtrice_indexed.h"

#include "engine/Actor/Actor.h"
#include "engine/Graphics/Sprite.h"
#include "engine/Input/InputHandler.h"

Sprite * player_sprite;
Actor * player_;

enum PLAYER_ANIMATIONS{
    IDLE,
    WALK,
    FIRE
};

// Input callbacks
void moveUp() {
    player_->setPosition(player_->getX(), player_->getY() - 1);
    player_->playAnimation(WALK, true);
}
void moveDown() {
    player_->setPosition(player_->getX(), player_->getY() + 1);
    player_->playAnimation(WALK, true);
}
void moveLeft() {
    player_->setPosition(player_->getX() - 1, player_->getY());
    player_->playAnimation(WALK, true);
}
void moveRight() {
    player_->setPosition(player_->getX() + 1, player_->getY());
    player_->playAnimation(WALK, true);
}
void turnLeft(){
    player_sprite->flipX(1);
}
void turnRight(){
    player_sprite->flipX(0);
}
void setIdle(){
    player_->playAnimation(IDLE, true);
}
void fireWand(){
    player_->playAnimation(FIRE, false);
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

    player_sprite = new Sprite(ATTR0_COLOR_16 | ATTR0_NORMAL | ATTR0_SQUARE,
        ATTR1_SIZE_32,
        ATTR2_PRIORITY(0) | ATTR2_PALETTE(0),
        2
    );
    player_sprite->setGFXIndex(1);
    Actor player(player_sprite);
    player_ = &player;
    player.setPosition(80, 60);

    auto clock = GbaClock::instance();

    // create animation sequence
    {
        std::vector<u32> idle;
        for(auto i = 0; i < 10; i++)
            idle.push_back(1 + i * 16);
        player.addAnimation(IDLE, std::move(idle), gba_milliseconds(200));

        //add walk anim
        std::vector<u32> walk;
        for(auto i = 20; i < 30; i++)
            walk.push_back(1 + i * 16);
        player.addAnimation(WALK, std::move(walk), gba_milliseconds(100));

        // add  fire animation
        std::vector<u32> fire;
        for(auto i = 30; i < 40; i++)
            fire.push_back(1 + i * 16);
        player.addAnimation(FIRE, std::move(fire), gba_milliseconds(100));
    }

    player.setStopAnimCallback(setIdle);
    player.playAnimation(0, true);

    // Initialize InputHandler
    InputHandler inputHandler;
    inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_UP, moveUp);
    inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_DOWN, moveDown);
    inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_LEFT, moveLeft);
    inputHandler.registerEvent(InputHandler::EventType::ButtonHeld, KEY_RIGHT, moveRight);

    inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_UP, setIdle);
    inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_DOWN, setIdle);
    inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_LEFT, setIdle);
    inputHandler.registerEvent(InputHandler::EventType::ButtonReleased, KEY_RIGHT, setIdle);

    inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_LEFT, turnLeft);
    inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_RIGHT, turnRight);

    inputHandler.registerEvent(InputHandler::EventType::ButtonPressed, KEY_A, fireWand);

    u32 count = 0;
    // Main game loop
    auto last_fram_time = clock.now();
    while (1) {
        count++;
        // Update InputHandler
        inputHandler.update();
        player.update(std::chrono::duration_cast<gba_milliseconds>(clock.now() - last_fram_time));

        last_fram_time = clock.now();
        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
