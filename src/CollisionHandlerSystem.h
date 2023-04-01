#ifndef COLLISION_HANDLER_SYSTEM_H
#define COLLISION_HANDLER_SYSTEM_H

#include "Sprites/wizard_spritesheet_calciumtrice_indexed.h"
#include "Sprites/shapes.h"

#include "engine/ECS/Components/EngineComponents.h"
#include "engine/ECS/System.h"

class CollisionHandlerSystem: public ISystem{
public:
    CollisionHandlerSystem(Entity player, Entity box): player_(player), box_(box){}
    void initialize(EntityManager& entityManager);
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;
private:
    Entity box_;
    Entity player_;
    
    std::shared_ptr<CollisionEventComponent> collisionEventsComponent;
    std::shared_ptr<PhysicsEventComponent> physicsEventsComponent;
};

#endif // COLLISION_HANDLER_SYSTEM_H