#pragma once

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class PhysicsSystem: public ISystem {
public:
    void initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

    static void updatePhysicsForm(EntityManager& entityManager, Entity entity, FormName name, cpSpace* space);
    static void removePhysicsFormFromSpace(EntityManager& entityManager, Entity entity, cpSpace* space);
};
