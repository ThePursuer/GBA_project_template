#ifndef COLLIDERI_H
#define COLLIDERI_H

#include "engine/Math/Vector.h"

class Collider {
public:
    virtual ~Collider() {}

    // Check for collision between this shape and another Collider object
    virtual bool __attribute__((section(".iwram"), long_call)) collidesWith(const Collider& other) const = 0;
    virtual Vector2 __attribute__((section(".iwram"), long_call)) getCollisionNormal(const Collider& other) const = 0;
    virtual Vector2 __attribute__((section(".iwram"), long_call)) getPenetration(const Collider& other) const = 0;
};

#endif // COLLIDERI_H