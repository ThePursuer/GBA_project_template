#include "engine/Math/RigidBody.h"

namespace gb_physics {

// Function definitions
void Rigidbody::applyImpulse(fix16_t impulseX, fix16_t impulseY) {
    if(isKinematic_)
        return;
    velocity_.x = fix16_add(velocity_.x, fix16_div(impulseX, mass_));
    velocity_.y = fix16_add(velocity_.y, fix16_div(impulseY, mass_));
}

void Rigidbody::setVelocity(fix16_t vx, fix16_t vy) {
    if(isKinematic_)
        return;
    velocity_.x = vx;
    velocity_.y = vy;
}

fix16_t Rigidbody::getSpeed() const {
    return fix16_sqrt(fix16_add(fix16_mul(velocity_.x, velocity_.x), fix16_mul(velocity_.y, velocity_.y)));
}

fix16_t Rigidbody::getInvMass() const {
    if (mass_ == F16(0.0)) {
        return F16(0.0);
    }
    return fix16_div(F16(1.0), mass_);
}

void Rigidbody::setVelocityX(fix16_t newVelocityX) { 
    if(isKinematic_)
        return;
    velocity_.x = newVelocityX;
}

void Rigidbody::setVelocityY(fix16_t newVelocityY) {
    if(isKinematic_)
        return;
    velocity_.y = newVelocityY;
}

void Rigidbody::update(gba_microseconds delta) {
    if (isKinematic_)
        return;

    gba_milliseconds dt_milliseconds = std::chrono::duration_cast<gba_milliseconds>(delta);
    // Calculate the time passed in seconds
    fix16_t seconds = fix16_div(fix16_from_int(dt_milliseconds.count()), F16(1000));

    // Apply drag to the velocity
    velocity_ = velocity_ * drag_;

    // Update the position based on the velocity and time passed
    *position_ = *position_ + (velocity_ * seconds) * F16(5);
}

} // namespace gb_physics
