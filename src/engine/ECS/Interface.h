#ifndef INTERFACE_H
#define INTERFACE_H

#include <gba_types.h>

#include "engine/Math/Vector.h"

class Position {
public:
    const s32& getX() const { return x_; }
    const s32& getY() const { return y_; }
    void setX(s32 x) { x_ = x; }
    void setY(s32 y) { y_ = y; }

    void setPosition(s32 x, s32 y) {
        x_ = x;
        y_ = y;
    }

    void setPosition(Vector2 pos) {
        x_ = fix16_to_int(pos.x);
        y_ = fix16_to_int(pos.y);
    }

private:
    s32 x_, y_;
};

#endif // INTERFACE_H