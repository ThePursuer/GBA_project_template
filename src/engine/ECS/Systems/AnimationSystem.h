#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <gba_types.h>

#include "engine/ECS/EntityManager.h"
#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

#include "engine/Graphics/SpriteManager.h"

class AnimationSystem : public ISystem {
public:
    void initialize(EntityManager& entityManager) override;
    void IWRAM_CODE update(EntityManager& entityManager, gba_microseconds deltaTime);
    static void playAnimation(EntityManager& entityManager, Entity entity, AnimationName name, bool loop);
    static void stopAnimation(EntityManager& entityManager, Entity entity);
    void shutdown(EntityManager& entityManager) override {}
    std::unordered_set<ComponentType> requiredComponents() const override;
private:
    static void stopAnimation(std::shared_ptr<AnimationClipComponent> animationComponent, std::shared_ptr<SpriteComponent> spriteComponent);
};

#endif // ANIMATION_SYSTEM_H