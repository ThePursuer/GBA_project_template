#pragma once 

#include <cmath>

#include <gba_base.h>

#include "engine/Math/Vector.h"
#include "engine/Math/Math.h"

class Rigidbody {
public:
    Rigidbody(float mass): mass_(mass), velocityX_(0), velocityY_(0), isKinematic_(false) {}
    Rigidbody(float mass, bool isKinematic): mass_(mass), velocityX_(0), velocityY_(0), isKinematic_(isKinematic) {}

    void IWRAM_CODE applyImpulse(float impulseX, float impulseY);
    void IWRAM_CODE setVelocity(float vx, float vy);

    float IWRAM_CODE getSpeed() const;
    Vector2 IWRAM_CODE getVelocity() const;

    // Getters
    bool IWRAM_CODE isKinematic() const { return isKinematic_; }
    float IWRAM_CODE getMass() const { return mass_; }
    float IWRAM_CODE getVelocityX() const { return velocityX_; }
    float IWRAM_CODE getVelocityY() const { return velocityY_; }
    float IWRAM_CODE getDrag() const { return drag_; }
    float IWRAM_CODE getInvMass() const;
    float IWRAM_CODE getFrictionCoefficient() const { return frictionCoefficient_; }

    // Setters
    void setKinematic(bool isKinematic) { isKinematic_ = isKinematic; }
    void IWRAM_CODE setMass(float newMass) { mass_ = newMass; }
    void IWRAM_CODE setVelocityX(float newVelocityX);
    void IWRAM_CODE setVelocityY(float newVelocityY);
    void IWRAM_CODE setDrag(float newDrag) { drag_ = newDrag; }
    void IWRAM_CODE setFrictionCoefficient(float frictionCoefficient) { frictionCoefficient_ = frictionCoefficient; }

private:
    bool isKinematic_;
    float mass_;
    float drag_ = 0.05f;
    float velocityX_, velocityY_;
    float frictionCoefficient_;
};
