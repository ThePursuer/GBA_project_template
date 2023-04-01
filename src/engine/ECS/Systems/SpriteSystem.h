#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class SpriteSystem: public ISystem{
public:
    void initialize(EntityManager& entityManager);
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    void __attribute__((section("iwram"))) shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

};

#endif // SPRITE_SYSTEM_H