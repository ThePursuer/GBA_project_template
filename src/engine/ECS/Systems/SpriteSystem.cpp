#include "engine/ECS/Systems/SpriteSystem.h"

void SpriteSystem::initialize(EntityManager& entityManager){
    Entity ICEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT)[0];
    auto ICComponent = std::static_pointer_cast<IndexableComponentsComponent>(entityManager.getComponent(ICEntity, EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT));
    auto required = requiredComponents();
    ICComponent->indexableComponents.insert(required.begin(), required.end());
}

void SpriteSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto entities = entityManager.getEntitiesWithComponent(EngineReservedComponents::SPRITE);
    for (Entity entity : entities){
        auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));
        spriteComponent->sprite->update();
    }
}

void SpriteSystem::shutdown(EntityManager& entityManager){

}

std::unordered_set<ComponentType> SpriteSystem::requiredComponents() const {
    return {EngineReservedComponents::SPRITE};
}