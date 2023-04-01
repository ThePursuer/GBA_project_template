#include "engine/ECS/Systems/CollisionSystem.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager){
    collisionEvents = entityManager.CreateEntity();
    collisionEventComponent = std::make_shared<CollisionEventComponent>();
    entityManager.addComponent(collisionEvents, EngineReservedComponents::COLLISION_EVENTS, collisionEventComponent);
}

void CollisionSystem::initialize(EntityManager& entityManager){
    Entity ICEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT)[0];
    auto ICComponent = std::static_pointer_cast<IndexableComponentsComponent>(entityManager.getComponent(ICEntity, EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT));
    auto required = requiredComponents();
    ICComponent->indexableComponents.insert(required.begin(), required.end());
}

void CollisionSystem::update(EntityManager& entityManager, gba_microseconds dt) {
    auto staticColliderEntities = entityManager.getEntitiesWithComponent({EngineReservedComponents::STATIC_COLLIDER});
    auto animationClipEntities = entityManager.getEntitiesWithComponent({EngineReservedComponents::ANIMATION_CLIP});

    std::vector<std::pair<std::shared_ptr<Collider>, Entity>> collidersWithEntities;

    // Pre-allocate memory for collidersWithEntities
    collidersWithEntities.reserve(staticColliderEntities.size() + animationClipEntities.size());

    // Get colliders from AnimationClipComponent entities
    for (const auto& entity : animationClipEntities) {
        auto animationClipComponent = std::static_pointer_cast<AnimationClipComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION_CLIP));


        if (animationClipComponent->currentClip != 0) {
            auto clip = animationClipComponent->clips[animationClipComponent->currentClip];
            if(clip.collider){
                collidersWithEntities.emplace_back(clip.collider, entity);
            }
            auto it = std::find(staticColliderEntities.begin(), staticColliderEntities.end(), entity);
            if (it != staticColliderEntities.end()){
                staticColliderEntities.erase(it);
            }
        }
    }

    // Get colliders from StaticColliderComponent entities
    for (const auto& entity : staticColliderEntities) {
        auto staticColliderComponent = std::static_pointer_cast<StaticColliderComponent>(entityManager.getComponent(entity, EngineReservedComponents::STATIC_COLLIDER));
        if(staticColliderComponent->collider)
            collidersWithEntities.emplace_back(staticColliderComponent->collider, entity);
    }

    auto collisioneventsentity = entityManager.getEntitiesWithComponent(EngineReservedComponents::COLLISION_EVENTS)[0];
    auto comp = std::static_pointer_cast<CollisionEventComponent>(entityManager.getComponent(collisioneventsentity, EngineReservedComponents::COLLISION_EVENTS));
    
    // Check for collisions between colliders
    for (size_t i = 0; i < collidersWithEntities.size() - 1; ++i) {
        auto colliderA = std::get<0>(collidersWithEntities[i]);
        auto& entityA = std::get<1>(collidersWithEntities[i]);

        for (size_t j = i + 1; j < collidersWithEntities.size(); ++j) {
            auto colliderB = std::get<0>(collidersWithEntities[j]);
            auto& entityB = std::get<1>(collidersWithEntities[j]);
            auto posA = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(entityA, EngineReservedComponents::POSITION));
            auto posB = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(entityB, EngineReservedComponents::POSITION));
            
            if(colliderA->collidesWith(*colliderB)){
                comp->events.push_back({entityA, colliderA, entityB, colliderB});
            }
        }
    }
}

void CollisionSystem::shutdown(EntityManager& entityManager){

}
