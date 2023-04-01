#include "engine/ECS/Systems/PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EntityManager& entityManager){
    Entity collisionEventComponentEntity = entityManager.CreateEntity();
    physicsEventsComponent = std::make_shared<PhysicsEventComponent>();
    entityManager.addComponent(collisionEventComponentEntity, EngineReservedComponents::PHYSICS_EVENTS, physicsEventsComponent);
}

void PhysicsSystem::initialize(EntityManager& entityManager){
    Entity collisionEventComponentEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::COLLISION_EVENTS)[0];
    collisionEventsComponent = std::static_pointer_cast<CollisionEventComponent>(entityManager.getComponent(collisionEventComponentEntity, EngineReservedComponents::COLLISION_EVENTS));

    Entity ICEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT)[0];
    auto ICComponent = std::static_pointer_cast<IndexableComponentsComponent>(entityManager.getComponent(ICEntity, EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT));
    auto required = requiredComponents();
    ICComponent->indexableComponents.insert(required.begin(), required.end());
}

IWRAM_CODE void PhysicsSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    auto rigidBodyEntities = entityManager.getEntitiesWithComponent(EngineReservedComponents::RIGID_BODY);
    for (auto it = rigidBodyEntities.begin(); it != rigidBodyEntities.end(); it++){
        auto rb = std::static_pointer_cast<RigidBodyComponent>(entityManager.getComponent(*it, EngineReservedComponents::RIGID_BODY));
        float dragX = -rb->body->getVelocityX() * rb->body->getDrag();
        float dragY = -rb->body->getVelocityY() * rb->body->getDrag();

        float newVelocityX = rb->body->getVelocityX() + dragX * (float)std::chrono::duration_cast<gba_milliseconds>(deltaTime).count();
        float newVelocityY = rb->body->getVelocityY() + dragY * (float)std::chrono::duration_cast<gba_milliseconds>(deltaTime).count();

        rb->body->setVelocity(newVelocityX, newVelocityY);
    }

    bool boxIsTouchingPlayer = false;
    auto it = collisionEventsComponent->events.begin();
    while(it != collisionEventsComponent->events.end()){
        auto & event = *it;
        auto rbA = std::static_pointer_cast<RigidBodyComponent>(entityManager.getComponent(event.A, EngineReservedComponents::RIGID_BODY))->body;
        auto rbB = std::static_pointer_cast<RigidBodyComponent>(entityManager.getComponent(event.B, EngineReservedComponents::RIGID_BODY))->body;

        // Position correction to prevent overlap
        auto aPosition = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(event.A, EngineReservedComponents::POSITION));
        auto bPosition = std::static_pointer_cast<PositionComponent>(entityManager.getComponent(event.B, EngineReservedComponents::POSITION));

        Vector2 relativeVelocity = rbA->getVelocity() - rbB->getVelocity();
        if (relativeVelocity.length() > 1.0) {
            auto collisionNormal = it->colliderA->getCollisionNormal(*it->colliderB);
            float normalVelocity = relativeVelocity.dot(collisionNormal);
            if (normalVelocity > 0) {
                // handle the collision kinematically rather than elastically.
                // This prevents objects from clipping through each other after a collision.
                // For performance reasons we will use goto here. 
                goto handleKinematic;
            }

            // Calculate and apply the normal impulse to both bodies
            float impulseScalar = -(0.05) * normalVelocity / (rbA->getInvMass() + rbB->getInvMass());
            Vector2 normalImpulse = collisionNormal * impulseScalar;
            rbA->applyImpulse(normalImpulse.x, normalImpulse.y);
            rbB->applyImpulse(-normalImpulse.x, -normalImpulse.y);
        }
        else {
            handleKinematic:; // goto handleKinematic;
            Vector2 penetration = event.colliderA->getPenetration(*event.colliderB);
            float ratio;
            if (rbA->isKinematic())
                ratio = 1;
            else if(rbB->isKinematic())
                ratio = 0;
            else
                ratio = rbA->getMass() / (rbA->getMass() + rbB->getMass());

            Vector2 aCorrected = penetration * (1 - ratio);
            aPosition->superPositionX += aCorrected.x;
            aPosition->superPositionY += aCorrected.y;

            Vector2 bCorrected = penetration * ratio;
            bPosition->superPositionX -= bCorrected.x;
            bPosition->superPositionY -= bCorrected.y;
        }
        
        it = collisionEventsComponent->events.erase(it);
    }
}

void PhysicsSystem::shutdown(EntityManager& entityManager){

}

std::unordered_set<ComponentType> PhysicsSystem::requiredComponents() const{
    return {EngineReservedComponents::COLLISION_EVENTS, EngineReservedComponents::RIGID_BODY, EngineReservedComponents::STATIC_COLLIDER, EngineReservedComponents::ANIMATION_CLIP};
}