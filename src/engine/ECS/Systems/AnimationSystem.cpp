#include "engine/ECS/Systems/AnimationSystem.h"

void AnimationSystem::initialize(EntityManager& entityManager) {
    Entity ICEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT)[0];
    auto ICComponent = std::static_pointer_cast<IndexableComponentsComponent>(entityManager.getComponent(ICEntity, EngineReservedComponents::INDEXABLE_COMPONENTS_COMPONENT));
    auto required = requiredComponents();
    ICComponent->indexableComponents.insert(required.begin(), required.end());
}

void AnimationSystem::update(EntityManager& entityManager, gba_microseconds deltaTime) {
    auto entities = entityManager.getEntitiesWithComponents({EngineReservedComponents::ANIMATION_CLIP, EngineReservedComponents::SPRITE});
    static u32 index = 1;

    for (const auto& entity : entities) {
        auto animationComponent = std::static_pointer_cast<AnimationClipComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION_CLIP));
        auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

        if (animationComponent->currentClip != 0) {
            // Update animation time

            animationComponent->elapsedTime += std::chrono::duration_cast<gba_milliseconds>(deltaTime);
            // Calculate the next frame index based on the current animation time
            u32 newFrameIndex = static_cast<u32>(animationComponent->elapsedTime.count() / animationComponent->clips[animationComponent->currentClip].clipDuration.count());

            // Check if the animation should loop or stop
            if (newFrameIndex >= animationComponent->clips[animationComponent->currentClip].frames.size()) {
                if (animationComponent->loopAnimation) {
                    newFrameIndex %= animationComponent->clips[animationComponent->currentClip].frames.size();
                    animationComponent->elapsedTime = gba_milliseconds::zero();
                } else {
                    stopAnimation(entityManager, entity);
                    continue;
                }
            }

            // Update the frame index and set the sprite's GFX index
            if (newFrameIndex != animationComponent->currentFrameIndex) {
                animationComponent->currentFrameIndex = newFrameIndex;            
                spriteComponent->sprite->setGFXIndex(animationComponent->clips[animationComponent->currentClip].frames[animationComponent->currentFrameIndex]);
                spriteComponent->needs_update = true;
            }
        }
    }
}


void AnimationSystem::playAnimation(EntityManager& entityManager, Entity entity, AnimationName name, bool loop) {
    auto animationComponent = std::static_pointer_cast<AnimationClipComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION_CLIP));
    auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

    if (!animationComponent) {    
        return; // AnimationComponent not found for the given entity
    }
    if (animationComponent->currentClip == name && animationComponent->currentClip != -1) {
        return; // The requested animation is already playing
    }

    if(animationComponent->currentClip != -1)
        stopAnimation(animationComponent, spriteComponent);

    animationComponent->currentClip = name;
    u32 baseIndex = SpriteManager::getInstance().loadAnimation(animationComponent->clips[animationComponent->currentClip]);
    setAnimationFrames(baseIndex, *spriteComponent->sprite.get(), animationComponent->clips[animationComponent->currentClip]);
    animationComponent->loopAnimation = loop;
    animationComponent->elapsedTime = gba_milliseconds::zero();
    animationComponent->currentFrameIndex = 0;
    spriteComponent->needs_update = true;
}

void AnimationSystem::stopAnimation(EntityManager& entityManager, Entity entity) {
    auto animationComponent = std::static_pointer_cast<AnimationClipComponent>(entityManager.getComponent(entity, EngineReservedComponents::ANIMATION_CLIP));
    auto spriteComponent = std::static_pointer_cast<SpriteComponent>(entityManager.getComponent(entity, EngineReservedComponents::SPRITE));

    if (!animationComponent || !spriteComponent) {
        return; // AnimationComponent not found for the given entity
    }
    stopAnimation(animationComponent, spriteComponent);
}

void AnimationSystem::stopAnimation(std::shared_ptr<AnimationClipComponent> animationComponent, std::shared_ptr<SpriteComponent> spriteComponent) {
    SpriteManager::getInstance().unloadSprite(animationComponent->clips[animationComponent->currentClip].frames[0]);
    animationComponent->elapsedTime = gba_milliseconds::zero();
    animationComponent->currentFrameIndex = 0;
    animationComponent->currentClip = 0;
    spriteComponent->needs_update = true;
}


std::unordered_set<ComponentType> AnimationSystem::requiredComponents() const {
    return { 
        EngineReservedComponents::ANIMATION_CLIP, 
        EngineReservedComponents::SPRITE
    };
}