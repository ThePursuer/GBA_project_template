#pragma once

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class PositionSystem: public ISystem {
    void IWRAM_CODE initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void IWRAM_CODE shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> IWRAM_CODE requiredComponents() const;
};
