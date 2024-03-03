#ifndef SPRITE_H
#define SPRITE_H

#include <gba_types.h>
#include <gba_sprites.h>
#include <gba_video.h>
#include <cstring>

#include "engine/ECS/Interface.h"

class SpriteMine {
public:
    SpriteMine(u16 attr0, u16 attr1, u16 attr2, u32 id, const Position& position);

    ~SpriteMine(); // Removes OAM entry
    void update();
    void setGFXIndex(u32 index);
    const s32 getX() const {return position_.getX();}
    const s32 getY() const {return position_.getY();}
    const s32 getObjIndexIncrement() const {return objIndexIncrement_;}
    void flipX(bool val);
    void flipY(bool val);

protected:
    /**
     * \name OBJ_INDEX_INCREMENT
     * \brief This is the increment for calculating a character name in an animation. For example if your sprite is OBJ_SQUARE size 32, you can use the OBJ_SQUARE32 to calculate the next index of the animation. Your 4th frame would be index = (START_FRAME_INDEX + 4 * OBJ_SQUARE32). Then do sprite.setGFXIndex(index)
    */
    enum OBJ_INDEX_INCREMENT{
        OBJ_RECT8 = 128 / 64,
        OBJ_SQUARE8 = 64 / 64,
        OBJ_RECT16 = 256 / 64,
        OBJ_SQUARE16 = 256 / 64,
        OBJ_RECT32 = 512 / 64,
        OBJ_SQUARE32 = 1024 / 64,
        OBJ_RECT64 = 2048 / 64,
        OBJ_SQUARE64 = 4096 / 64
    };

    const Position& position_;
    u32 id_; // Index of the sprite in the OAM
    u32 sprite_index_;
    u32 objIndexIncrement_;

    // Sprite attributes
    u16 spriteAttr0_; // Attribute 0 (Y coordinate, shape, etc.)
    u16 spriteAttr1_; // Attribute 1 (X coordinate, size, etc.)
    u16 spriteAttr2_; // Attribute 2 (palette, priority, etc.)
};

#endif // SPRITE_H