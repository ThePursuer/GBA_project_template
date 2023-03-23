#include "engine/ECS/Systems/SpriteSystem.h"

void SpriteSystem::initialize(EntityManager& entityManager){
    
}

void SpriteSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto entities = entityManager.getEntitiesWithComponent(EngineReservedComponents::SPRITE);
    for (Entity entity : entities){
        auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));
        if (spriteComponent->needs_update)
            spriteComponent->sprite->update();
    }
}

void SpriteSystem::shutdown(EntityManager& entityManager){

}

std::set<ComponentType> SpriteSystem::requiredComponents() const {
    return {EngineReservedComponents::SPRITE};
}