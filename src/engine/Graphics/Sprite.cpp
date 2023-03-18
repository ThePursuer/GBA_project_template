#include "Sprite.h"

Sprite::~Sprite(){
    memset(&OAM[id_].attr0, 0, 6);
}

void Sprite::update() {
    // Update the sprite's OAM data
    OAM[id_].attr0 = spriteAttr0 | (y & 0xFF);
    OAM[id_].attr1 = spriteAttr1 | (x & 0x1FF);
    OAM[id_].attr2 = spriteAttr2 | (sprite_index & 0x3FF);
}

void Sprite::setPosition(s32 x, s32 y) {
    this->x = x;
    this->y = y;
}

void Sprite::setGFXIndex(u32 index) {
    sprite_index = index;
}

void Sprite::flipX(bool val){
    if (val) {
        spriteAttr1 |= (1 << 12);
    } else {
        spriteAttr1 &= ~(1 << 12);
    }
}

void Sprite::flipY(bool val){
    if (val) {
        spriteAttr1 |= (1 << 13);
    } else {
        spriteAttr1 &= ~(1 << 13);
    }
}
