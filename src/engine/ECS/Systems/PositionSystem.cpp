#include "engine/ECS/Systems/PositionSystem.h"

void PositionSystem::initialize(EntityManager& entityManager){
    
}

void PositionSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto positionalEntities = entityManager.getEntitiesWithComponent(EngineReservedComponents::POSITION);
    for(auto& entity: positionalEntities){
        auto positionComponent  = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(entity, EngineReservedComponents::POSITION));
        if (positionComponent->changed) {
            if (entityManager.hasComponent(entity, EngineReservedComponents::SPRITE)){
                auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));
                spriteComponent->needs_update = true;
            }
            positionComponent->changed = false;
        }
        
        // Always update if we have a physics related item
        if (positionComponent->position_)
            positionComponent->setPosition(*positionComponent->position_);
    }
}

void PositionSystem::shutdown(EntityManager& entityManager){
    
}

std::unordered_set<ComponentType> PositionSystem::requiredComponents() const{
    return {EngineReservedComponents::POSITION};
}
