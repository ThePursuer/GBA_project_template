#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "ecs/EntityManager.h"
#include "ecs/System.h"

#include <GBA_OS/Clock/GbaClock.h>

class SystemManager {
public:
    void addSystem(ISystem* system);

    void initializeAllSystems(EntityManager& entityManager);

    void updateAllSystems(EntityManager& entityManager, gba_os::chrono::gba_microseconds deltaTime);

    void shutdownAllSystems(EntityManager& entityManager);

private:
    std::vector<ISystem*> systems;
};

#endif // SYSTEMMANAGER_H