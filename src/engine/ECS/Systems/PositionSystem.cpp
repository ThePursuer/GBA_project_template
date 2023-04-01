#include "engine/ECS/Systems/PositionSystem.h"

void PositionSystem::initialize(EntityManager& entityManager){
    
}

void PositionSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto positionalEntities = entityManager.getEntitiesWithComponent(EngineReservedComponents::POSITION);
    for(auto& entity: positionalEntities){
        auto positionComponent  = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(entity, EngineReservedComponents::POSITION));
        positionComponent->x = positionComponent->getSuperPositionX();
        positionComponent->y = positionComponent->getSuperPositionY();
    }
}

void PositionSystem::shutdown(EntityManager& entityManager){
    
}

std::unordered_set<ComponentType> PositionSystem::requiredComponents() const{
    return {EngineReservedComponents::POSITION};
}
