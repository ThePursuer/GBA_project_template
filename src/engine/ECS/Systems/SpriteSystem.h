#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class SpriteSystem: ISystem{
public:
    void initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

};

#endif // SPRITE_SYSTEM_H