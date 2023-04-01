#include "engine/Math/Physics.h"

// Function definitions
void Rigidbody::applyImpulse(float impulseX, float impulseY) {
    if(isKinematic_)
        return;
    velocityX_ += impulseX / mass_;
    velocityY_ += impulseY / mass_;
}

void Rigidbody::setVelocity(float vx, float vy) {
    if(isKinematic_)
        return;
    velocityX_ = vx;
    velocityY_ = vy;
}

Vector2 Rigidbody::getVelocity() const {
    return Vector2(velocityX_, velocityY_);
}

float Rigidbody::getSpeed() const {
    return Math::sqrtFloat(velocityX_ * velocityX_ + velocityY_ * velocityY_);
}

float IWRAM_CODE Rigidbody::getInvMass() const {
    if (mass_ == 0) {
        return 0;
    }
    return 1.0f / mass_;
}

void Rigidbody::setVelocityX(float newVelocityX) { 
    if(isKinematic_)
        return;
    velocityX_ = newVelocityX;
}

void Rigidbody::setVelocityY(float newVelocityY) {
    if(isKinematic_)
        return;
    velocityY_ = newVelocityY;
}
