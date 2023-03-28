#include "CollisionHandlerSystem.h"
extern u32 shapes;

void CollisionHandlerSystem::initialize(EntityManager& entityManager) {
    Entity collisionEventComponentEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::COLLISION_EVENTS)[0];
    collisionEventsComponent = std::static_pointer_cast<CollisionEventComponent>(entityManager.getComponent(collisionEventComponentEntity, EngineReservedComponents::COLLISION_EVENTS));
}

#include <stdio.h>
void CollisionHandlerSystem::update(EntityManager& entityManager, gba_microseconds deltaTime) {
    bool boxIsTouchingPlayer = false;
    auto it = collisionEventsComponent->events.begin();
    while(it != collisionEventsComponent->events.end()){
        auto & event = *it;
        if ((event.A == player_ || event.B == player_) && (event.A == box_ || event.B == box_)){
            auto spriteComp = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(box_, EngineReservedComponents::SPRITE));
            spriteComp->sprite->setGFXIndex(shapes);
            spriteComp->needs_update = true;
            boxIsTouchingPlayer = true;
        }
        collisionEventsComponent->events.erase(it);
    }

    if(!boxIsTouchingPlayer){
        auto spriteComp = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(box_, EngineReservedComponents::SPRITE));
        spriteComp->sprite->setGFXIndex(shapes + spriteComp->sprite->getObjIndexIncrement());
        spriteComp->needs_update = true;
    }
}

void CollisionHandlerSystem::shutdown(EntityManager& entityManager ){

}

std::unordered_set<ComponentType> CollisionHandlerSystem::requiredComponents() const {
    return {EngineReservedComponents::STATIC_COLLIDER, EngineReservedComponents::ANIMATION_CLIP, EngineReservedComponents::COLLISION_EVENTS};
}
