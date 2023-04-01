#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"
#include "engine/Signal.h"

class CollisionSystem : public ISystem{
public:
    CollisionSystem(EntityManager& entityManager);
    void initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const {return {EngineReservedComponents::ANIMATION_CLIP, EngineReservedComponents::STATIC_COLLIDER, EngineReservedComponents::COLLISION_EVENTS};}
private:
    Entity collisionEvents;
    std::shared_ptr<CollisionEventComponent> collisionEventComponent;
};

#endif // COLLISION_SYSTEM_H