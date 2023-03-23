#include "engine/ECS/Systems/AnimationSystem.h"

void AnimationSystem::update(EntityManager& entityManager, gba_microseconds deltaTime) {
    auto entities = entityManager.getEntitiesWithComponents({EngineReservedComponents::ANIMATION, EngineReservedComponents::SPRITE});
    static u32 index = 1;

    for (const auto& entity : entities) {
        auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));
        auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

        if (animationComponent->isAnimating_) {
            // Update animation time

            animationComponent->animationTime += std::chrono::duration_cast<gba_milliseconds>(deltaTime);
            // Calculate the next frame index based on the current animation time
            u32 newFrameIndex = static_cast<u32>(animationComponent->animationTime.count() / animationComponent->sequences_[animationComponent->currentAnimation].frameDuration_.count());

            // Check if the animation should loop or stop
            if (newFrameIndex >= animationComponent->sequences_[animationComponent->currentAnimation].frames_.size()) {
                if (animationComponent->loopAnimation) {
                    newFrameIndex %= animationComponent->sequences_[animationComponent->currentAnimation].frames_.size();
                    animationComponent->animationTime = gba_milliseconds::zero();
                } else {
                    stopAnimation(entityManager, entity);
                    continue;
                }
            }

            // Update the frame index and set the sprite's GFX index
            if (newFrameIndex != animationComponent->currentFrameIndex) {
                animationComponent->lastFrameIndex = animationComponent->currentFrameIndex;
                animationComponent->currentFrameIndex = newFrameIndex;            
                spriteComponent->sprite->setGFXIndex(animationComponent->sequences_[animationComponent->currentAnimation].frames_[animationComponent->currentFrameIndex]);
                spriteComponent->needs_update = true;
            }
        }
    }
}

void AnimationSystem::playAnimation(EntityManager& entityManager, Entity entity, AnimationName name, bool loop) {
    auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));
    auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

    if (!animationComponent) {    
        return; // AnimationComponent not found for the given entity
    }
    if (animationComponent->currentAnimation == name && animationComponent->isAnimating_) {
        return; // The requested animation is already playing
    }

    animationComponent->isAnimating_ = true;
    animationComponent->currentAnimation = name;
    animationComponent->loopAnimation = loop;
    animationComponent->animationTime = gba_milliseconds::zero();
    animationComponent->currentFrameIndex = 0;
    animationComponent->lastFrameIndex = 0;
    spriteComponent->needs_update = true;
}

void AnimationSystem::stopAnimation(EntityManager& entityManager, Entity entity) {
    auto animationComponent = std::static_pointer_cast<AnimationComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION));
    auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

    if (!animationComponent) {
        return; // AnimationComponent not found for the given entity
    }

    animationComponent->isAnimating_ = false;
    animationComponent->animationTime = gba_milliseconds::zero();
    animationComponent->currentFrameIndex = 0;
    animationComponent->lastFrameIndex = 0;
    animationComponent->currentAnimation = 0;
    spriteComponent->needs_update = true;
}

std::set<ComponentType> AnimationSystem::requiredComponents() const {
    return { 
        EngineReservedComponents::ANIMATION, 
        EngineReservedComponents::SPRITE
    };
}