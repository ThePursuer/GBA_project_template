#pragma once

#include "engine/Math/Vector.h"

namespace gb_physics {

class Shape {
public:
    virtual ~Shape() {}
    virtual const fix16_t x() const = 0;
    virtual const fix16_t y() const = 0;
    virtual bool collidesWith(Shape& shape) = 0;
    virtual Vector2 getPenetration(Shape& shape) = 0;
};

} // namespace gb_physics
