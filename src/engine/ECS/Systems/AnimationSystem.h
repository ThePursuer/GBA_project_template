#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <gba_types.h>

#include "engine/ECS/EntityManager.h"
#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

class AnimationSystem : public ISystem {
public:
    void initialize(EntityManager& entityManager) override {}
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    static void playAnimation(EntityManager& entityManager, Entity entity, AnimationName name, bool loop);
    static void stopAnimation(EntityManager& entityManager, Entity entity);
    void shutdown(EntityManager& entityManager) override {}
    std::set<ComponentType> requiredComponents() const override;
};

#endif // ANIMATION_SYSTEM_H