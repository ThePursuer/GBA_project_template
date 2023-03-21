#ifndef ENGINE_SYSTEMS_H
#define ENGINE_SYSTEMS_H

#include <gba_types.h>

#include "engine/ECS/EntityManager.h"
#include "engine/ECS/System.h"
#include "engine/ECS/EngineComponents.h"

class AnimationSystem : public ISystem {
public:
    void initialize(EntityManager& entityManager) override {
        // Initialization code
    }

    void update(EntityManager& entityManager, gba_milliseconds deltaTime) override {
        auto entities = entityManager.getEntitiesWithComponents({EngineReservedComponents::ANIMATION, EngineReservedComponents::SPRITE, EngineReservedComponents::POSITION});

        for (const auto& entity : entities) {
            auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));
            auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

            if (animationComponent->isAnimating_) {
                // Update animation time
                animationComponent->animationTime += deltaTime;
                // Calculate the next frame index based on the current animation time
                u32 newFrameIndex = static_cast<u32>(animationComponent->animationTime.count() / animationComponent->sequences_[animationComponent->currentAnimation].frameDuration_.count());

                // Check if the animation should loop or stop
                if (newFrameIndex >= animationComponent->sequences_[animationComponent->currentAnimation].frames_.size()) {
                    if (animationComponent->loopAnimation) {
                        newFrameIndex %= animationComponent->sequences_[animationComponent->currentAnimation].frames_.size();
                        animationComponent->animationTime = gba_milliseconds::zero();
                    } else {
                        animationComponent->isAnimating_ = false;
                        continue;
                    }
                }

                // Update the frame index and set the sprite's GFX index
                if (newFrameIndex != animationComponent->currentFrameIndex) {
                    animationComponent->currentFrameIndex = newFrameIndex;
                    spriteComponent->sprite->setGFXIndex(animationComponent->sequences_[animationComponent->currentAnimation].frames_[animationComponent->currentFrameIndex]);
                    animationComponent->should_update_sprite_ = true;
                }
            }

            // Update the sprite if the actor is animating or has moved
            if (animationComponent->should_update_sprite_) {
                spriteComponent->sprite->update();
                animationComponent->should_update_sprite_ = false;
            }
        }
    }

    static void playAnimation(EntityManager& entityManager, Entity entity, AnimationName name, bool loop) {
        auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));

        if (!animationComponent) {
            return; // AnimationComponent not found for the given entity
        }

        if (animationComponent->currentAnimation == name) {
            return; // The requested animation is already playing
        }

        animationComponent->isAnimating_ = true;
        animationComponent->currentAnimation = name;
        animationComponent->loopAnimation = loop;
        animationComponent->animationTime = gba_milliseconds::zero();
        animationComponent->currentFrameIndex = 0;
        animationComponent->should_update_sprite_ = true;
    }

    void stopAnimation(EntityManager& entityManager, Entity entity) {
        auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));

        if (!animationComponent) {
            return; // AnimationComponent not found for the given entity
        }

        animationComponent->isAnimating_ = false;
        animationComponent->should_update_sprite_ = true;
    }

    void shutdown(EntityManager& entityManager) override {
        // Cleanup code
    }

    std::set<ComponentType> requiredComponents() const override {
        return { EngineReservedComponents::ANIMATION, EngineReservedComponents::SPRITE, EngineReservedComponents::POSITION };
    }
};

#endif // ENGINE_SYSTEMS_H