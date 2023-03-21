#ifndef SYSTEM_H
#define SYSTEM_H

#include "engine/ECS/EntityManager.h"

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void initialize(EntityManager& entityManager) = 0;
    virtual void update(EntityManager& entityManager, gba_milliseconds deltaTime) = 0;
    virtual void shutdown(EntityManager& entityManager) = 0;
    virtual std::set<ComponentType> requiredComponents() const = 0;
};

#endif // SYSTEM_H