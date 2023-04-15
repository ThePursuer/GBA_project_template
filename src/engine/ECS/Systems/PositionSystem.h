#pragma once

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class PositionSystem: public ISystem {
    void initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;
};
