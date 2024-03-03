#include "engine/Graphics/SpriteManager.h"

u32 SpriteManager::loadSprite(const void* spriteData, u32 size){
    void* memory = objVramManager.allocate(size);
    if (memory == nullptr)
        return 0;
    CpuFastSet(spriteData, memory, size / 4);

    // calclutate obj index
    u32 id = ((char*)memory - (char*)SPRITE_GFX) / 32;
    mappedSprites[id] = std::pair<void*, u32>(memory, size);
    return id;
}

u32 SpriteManager::loadAnimation(AnimationClip& clip){
    void* memory = objVramManager.allocate(clip.graphicsDataLen);
    if (memory == nullptr)
        return 0;
    CpuFastSet(clip.graphicsData, memory, clip.graphicsDataLen  / 4);

    // calclutate obj index
    u32 id = ((char*)memory - (char*)SPRITE_GFX) / 32;
    mappedSprites[id] = std::pair<void*, u32>(memory, clip.graphicsDataLen);
    return id;
}

void SpriteManager::unloadSprite(u32 handle){
    objVramManager.free(mappedSprites[handle].first, mappedSprites[handle].second);
    mappedSprites.erase(handle);
}

void setAnimationFrames(u32 baseAnimationIndex, SpriteMine& sprite, AnimationClip& clip){
    clip.frames.clear();
    for(int i = baseAnimationIndex; i < (clip.frameCount * sprite.getObjIndexIncrement() + baseAnimationIndex); i+= sprite.getObjIndexIncrement())
        clip.frames.push_back(i);
}