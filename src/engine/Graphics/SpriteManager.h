#pragma once

#include <unordered_map>

#include <gba_sprites.h>
#include <gba_systemcalls.h>

#include "engine/Memory/MemoryAllocator.h"
#include "engine/Graphics/Animation.h"
#include "engine/Graphics/Sprite.h"

class SpriteManager {
public:
    // Returns the single instance of the SpriteManager class
    static SpriteManager& getInstance() {
        static SpriteManager instance;
        return instance;
    }

    u32 loadSprite(const void* spriteData, u32 size);
    u32 loadAnimation(AnimationClip& clip);
    void unloadSprite(u32 handle);
    void clearOAM(); // Clears OAM entry without deleting attr from instance

private:
    /**
     * \name SpriteManager()
     * \note SPRITE_GFX + 16 because SPRITE_GFX is reserved
     * \note (void*)SPRITE_GFX + 32768 the last available address in obj vram
    */
    SpriteManager(): objVramManager((char*)SPRITE_GFX + 32, (char*)SPRITE_GFX + 32768) {}

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;
    ~SpriteManager() {}

    MemoryManager objVramManager;
    std::unordered_map<u32, std::pair<void*, u32>> mappedSprites;
};

void setAnimationFrames(u32 baseAnimationIndex, Sprite& sprite, AnimationClip& clip);
