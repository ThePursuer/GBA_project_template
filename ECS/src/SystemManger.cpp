#include "ecs/SystemManager.h"


void SystemManager::addSystem(ISystem* system) {
    systems.push_back(system);
}

void SystemManager::initializeAllSystems(EntityManager& entityManager) {
    for (auto& system : systems) {
        system->initialize(entityManager);
    }
}

void SystemManager::updateAllSystems(EntityManager& entityManager, gba_os::chrono::gba_microseconds deltaTime) {
    for (auto& system : systems) {
        system->update(entityManager, deltaTime);
    }
}

void SystemManager::shutdownAllSystems(EntityManager& entityManager) {
    for (auto& system : systems) {
        system->shutdown(entityManager);
    }
}