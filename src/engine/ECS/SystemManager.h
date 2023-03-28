#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "engine/ECS/EntityManager.h"
#include "engine/ECS/System.h"
#include "engine/Clock/GbaClock.h"

class SystemManager {
public:
    void addSystem(ISystem* system) {
        systems.push_back(system);
    }

    void initializeAllSystems(EntityManager& entityManager) {
        for (auto& system : systems) {
            system->initialize(entityManager);
        }
    }

    void updateAllSystems(EntityManager& entityManager, gba_microseconds deltaTime) {
        for (auto& system : systems) {
            system->update(entityManager, deltaTime);
        }
    }

    void shutdownAllSystems(EntityManager& entityManager) {
        for (auto& system : systems) {
            system->shutdown(entityManager);
        }
    }

private:
    std::vector<ISystem*> systems;
};

#endif // SYSTEMMANAGER_H