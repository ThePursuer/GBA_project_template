#include "engine/ECS/Systems/PhysicsSystem.h"
#include "stdio.h"
void PhysicsSystem::initialize(EntityManager& entityManager){
    
}
void PhysicsSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto physicsEntities = entityManager.getEntitiesWithComponent(EngineReservedComponents::PHYSICS);
    for(Entity entity: physicsEntities){
        auto physicsComponent = std::static_pointer_cast<PhysicsComponent>(entityManager.getComponent(entity, EngineReservedComponents::PHYSICS));
        auto positionComponent = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(entity, EngineReservedComponents::POSITION));
        auto position = physicsComponent->forms[physicsComponent->currentForm].body->p;
        positionComponent->superPositionX = position.x;
        positionComponent->superPositionY = position.y;
    }
}

void PhysicsSystem::shutdown(EntityManager& entityManager){
    
}

std::unordered_set<ComponentType> PhysicsSystem::requiredComponents() const {
    return {EngineReservedComponents::PHYSICS};
}

void PhysicsSystem::updatePhysicsForm(EntityManager& entityManager, Entity entity, FormName name, cpSpace* space){
    auto physicsComp = std::static_pointer_cast<PhysicsComponent>(entityManager.getComponent(entity, EngineReservedComponents::PHYSICS));
    physicsComp->currentForm = name;
    
    cpSpaceAddBody(space, physicsComp->forms[name].body.get());
    cpSpaceAddShape(space, physicsComp->forms[name].shape.get());
}

void PhysicsSystem::removePhysicsFormFromSpace(EntityManager& entityManager, Entity entity, cpSpace* space){
    auto physicsComp = std::static_pointer_cast<PhysicsComponent>(entityManager.getComponent(entity, EngineReservedComponents::PHYSICS));

    cpSpaceRemoveBody(space, physicsComp->forms[physicsComp->currentForm].body.get());
    cpSpaceRemoveShape(space, physicsComp->forms[physicsComp->currentForm].shape.get());
}
