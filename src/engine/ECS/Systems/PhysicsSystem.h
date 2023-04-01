#pragma once

#include "engine/ECS/System.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Components/EngineComponents.h"

class PhysicsSystem: public ISystem {
public:
    PhysicsSystem(EntityManager& entityManager);
    void initialize(EntityManager& entityManager);
    IWRAM_CODE void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;
private:
    Entity physicsEventEntity;    
    std::shared_ptr<CollisionEventComponent> collisionEventsComponent;
    std::shared_ptr<PhysicsEventComponent> physicsEventsComponent;
};
