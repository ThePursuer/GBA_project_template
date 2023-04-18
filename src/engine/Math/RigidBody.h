#pragma once 

#include <cmath>
#include <memory>

#include <gba_base.h>
#include <libfixmath/fixmath.h>

#include "engine/Math/Vector.h"
#include "engine/Clock/GbaClock.h"

namespace gb_physics {

class Rigidbody {
public:
    Rigidbody(fix16_t mass, std::shared_ptr<Vector2> position): mass_(mass), velocity_(F16(0.0), F16(0.0)), isKinematic_(false), position_(position) {}
    Rigidbody(fix16_t mass, std::shared_ptr<Vector2> position, bool isKinematic): mass_(mass), velocity_(F16(0.0), F16(0.0)), isKinematic_(isKinematic), position_(position) {}

    void applyImpulse(fix16_t impulseX, fix16_t impulseY);
    void setVelocity(fix16_t vx, fix16_t vy);
    bool isKinematic() const { return isKinematic_; }
    void update(gba_microseconds delta);

    // Getters
    Vector2 getVelocity() const { return velocity_; }
    Vector2 getPosition() const { return *position_; }
    fix16_t getMass() const { return mass_; }
    fix16_t getDrag() const { return drag_; }
    fix16_t getSpeed() const;
    fix16_t getInvMass() const;
    fix16_t getFrictionCoefficient() const { return frictionCoefficient_; }

    // Setters
    void setKinematic(bool isKinematic) { isKinematic_ = isKinematic; }
    void setMass(fix16_t newMass) { mass_ = newMass; }
    void setVelocityX(fix16_t newVelocityX);
    void setVelocityY(fix16_t newVelocityY);
    void setPosition(Vector2 pos) {*position_ = pos;}
    void setDrag(fix16_t newDrag) { drag_ = newDrag; }
    void setFrictionCoefficient(fix16_t frictionCoefficient) { frictionCoefficient_ = frictionCoefficient; }

private:
    bool isKinematic_;
    fix16_t mass_;
    fix16_t drag_ = F16(0.0);
    fix16_t frictionCoefficient_;

    Vector2 velocity_;
    std::shared_ptr<Vector2> position_;
};

} // namespace gb_physics
