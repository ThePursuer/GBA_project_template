#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <vector>
#include <set>
#include <unordered_map>

#include <gba_types.h>

#include "engine/Graphics/Sprite.h"
#include "engine/Graphics/Animation.h"
#include "engine/Math/Collider.h"
#include "engine/Math/ColliderI.h"
#include "engine/Math/Physics.h"
#include "engine/Clock/GbaClock.h"
#include "engine/ECS/Component.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Interface.h"



class StaticColliderComponent: public Component{
public:
    std::shared_ptr<Collider> collider;
};

class AnimationClipComponent : public Component {
public:
    std::unordered_map<AnimationName, AnimationClip> clips;  // The animation clips for this entity
    AnimationName currentClip = -1;                           // The current animation clip being played
    u32 currentFrameIndex = 0;                           // The current frame index in the current animation clip
    gba_milliseconds elapsedTime = gba_milliseconds(0);  // The elapsed time since the current animation clip started
    bool loopAnimation = false;                          // Whether the current animation should loop
};

class SpriteComponent: public Component{
public:
    std::unique_ptr<Sprite> sprite;
    bool needs_update = true;
};

class PositionComponent: public Position, public Component{
public:
    const __attribute__((section(".iwram"), long_call)) s32& getX() const {return x;}
    const __attribute__((section(".iwram"), long_call)) s32& getY() const {return y;}
    const __attribute__((section(".iwram"), long_call)) float& getSuperPositionX() const {return superPositionX;}
    const __attribute__((section(".iwram"), long_call)) float& getSuperPositionY() const {return superPositionY;}
    float superPositionX, superPositionY; // Useful for modifying position by half pixels accross frames. Helps reduce physics bugs.
    s32 x, y;
};

struct CollisionEvent {
    Entity A;
    std::shared_ptr<Collider> colliderA;
    Entity B;
    std::shared_ptr<Collider> colliderB;
};

class CollisionEventComponent: public Component{
public:
    std::vector<CollisionEvent> events;
};

struct PhysicsEvent {
    PhysicsEvent() = default;
    PhysicsEvent(const CollisionEvent& event){
        this->A = event.A;
        this->B = event.B;
    }
    PhysicsEvent(const CollisionEvent& event, const float& restitution, const Vector2& collisionNormal): PhysicsEvent(event){
        this->restitution = restitution;
        this->collisionNormal = collisionNormal;
    }

    Entity A;
    std::shared_ptr<Rigidbody> rbA;
    Entity B;
    std::shared_ptr<Rigidbody> rbB;
    float restitution;
    Vector2 collisionNormal;
};

class PhysicsEventComponent: public Component{
public:
    std::vector<PhysicsEvent> events;
};

class RigidBodyComponent: public Component{
public:
    RigidBodyComponent(float mass): body(std::make_shared<Rigidbody>(mass)){}
    RigidBodyComponent(float mass, bool isKinematic): body(std::make_shared<Rigidbody>(mass, isKinematic)){}
    std::shared_ptr<Rigidbody> body;
};

#endif // ENGINE_COMPONENTS_H