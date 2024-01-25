#ifndef SYSTEM_H
#define SYSTEM_H

#include "ecs/EntityManager.h"

#include <GBA_OS/Clock/GbaClock.h>

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void initialize(EntityManager& entityManager) = 0;
    virtual void update(EntityManager& entityManager, gba_os::chrono::gba_microseconds deltaTime) = 0;
    virtual void shutdown(EntityManager& entityManager) = 0;
    virtual std::unordered_set<ComponentType> requiredComponents() const = 0;
};

#endif // SYSTEM_H