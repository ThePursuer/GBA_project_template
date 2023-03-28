#ifndef COLLIDERI_H
#define COLLIDERI_H

class Collider {
public:
    virtual ~Collider() {}

    // Check for collision between this shape and another Collider object
    virtual bool __attribute__((section(".iwram"), long_call)) collidesWith(const Collider& other) const = 0;
};

#endif // COLLIDERI_H