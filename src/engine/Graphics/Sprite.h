#ifndef SPRITE_H
#define SPRITE_H

#include <gba_types.h>
#include <gba_sprites.h>
#include <gba_video.h>
#include <cstring>

class Sprite {
public:
    Sprite(u16 attr0, u16 attr1, u16 attr2, u32 id)
        : x(0), y(0), id_(id), spriteAttr0(attr0), spriteAttr1(attr1), spriteAttr2(attr2) {}

    ~Sprite(); // Removes OAM entry
    void update();
    void setPosition(s32 x, s32 y);
    void setGFXIndex(u32 index);
    const s32 getX() const {return x;}
    const s32 getY() const {return y;}
    void flipX(bool val);
    void flipY(bool val);
    
protected:
    s32 x, y; // Position of the actor
    u32 id_; // Index of the sprite in the OAM
    u32 sprite_index;

    // Sprite attributes
    u16 spriteAttr0; // Attribute 0 (Y coordinate, shape, etc.)
    u16 spriteAttr1; // Attribute 1 (X coordinate, size, etc.)
    u16 spriteAttr2; // Attribute 2 (palette, priority, etc.)
};

#endif // SPRITE_H