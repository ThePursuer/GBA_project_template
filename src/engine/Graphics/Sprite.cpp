#include "Sprite.h"

Sprite::Sprite(u16 attr0, u16 attr1, u16 attr2, u32 id, const Position& position)
    : position_(position), id_(id), spriteAttr0_(attr0), spriteAttr1_(attr1), spriteAttr2_(attr2) {
        auto shape = (spriteAttr0_ >> 14) & 0b11;
        auto size = (spriteAttr1_ >> 14) & 0b11;
        switch (size) {
            case 0:
                objIndexIncrement_ = (shape == 0) ? OBJ_SQUARE8 : OBJ_RECT8;
                break;
            case 1:
                objIndexIncrement_ = (shape == 0) ? OBJ_SQUARE16 : OBJ_RECT16;
                break;
            case 2:
                objIndexIncrement_ = (shape == 0) ? OBJ_SQUARE32 : OBJ_RECT32;
                break;
            case 3:
                objIndexIncrement_ = (shape == 0) ? OBJ_SQUARE64 : OBJ_RECT64;
                break;
            default:
                break;
        }
}

Sprite::~Sprite(){
    memset(&OAM[id_].attr0, 0, 6);
}

void Sprite::update() {
    // Update the sprite's OAM data
    OAM[id_].attr0 = spriteAttr0_ | (getY() & 0xFF);
    OAM[id_].attr1 = spriteAttr1_ | (getX() & 0x1FF);
    OAM[id_].attr2 = spriteAttr2_ | (sprite_index_ & 0x3FF);
}

void Sprite::setGFXIndex(u32 index) {
    sprite_index_ = index;
}

void Sprite::flipX(bool val){
    if (val) {
        spriteAttr1_ |= (1 << 12);
    } else {
        spriteAttr1_ &= ~(1 << 12);
    }
}

void Sprite::flipY(bool val){
    if (val) {
        spriteAttr1_ |= (1 << 13);
    } else {
        spriteAttr1_ &= ~(1 << 13);
    }
}
