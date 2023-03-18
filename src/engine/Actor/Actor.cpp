#include "engine/Actor/Actor.h"


Actor::Actor(Sprite* sprite)
    : sprite_(sprite),
      currentAnimation(UINT32_MAX),
      loopAnimation(false),
      currentFrameIndex(0) {
}

void Actor::setPosition(s32 x, s32 y) {
    sprite_->setPosition(x, y); 
    should_update_sprite_ = true;
}

void Actor::addAnimation(AnimationName name, const std::vector<u32>& frameIndices, gba_milliseconds frameDuration) {
    animations[name] = frameIndices;
    frameDurations[name] = frameDuration;
}

void Actor::update(gba_milliseconds deltaTime) {
    if (isAnimating()) {
        // Update animation time
        animationTime += deltaTime;
        // Calculate the next frame index based on the current animation time
        u32 newFrameIndex = static_cast<u32>(animationTime.count() / frameDurations[currentAnimation].count());

        // Check if the animation should loop or stop
        if (newFrameIndex >= animations[currentAnimation].size()) {
            if (loopAnimation) {
                newFrameIndex %= animations[currentAnimation].size();
                animationTime = gba_milliseconds::zero();
            } else {
                stopAnimation();
            }
        }

        // Update the frame index and set the sprite's GFX index
        if (newFrameIndex != currentFrameIndex) {
            currentFrameIndex = newFrameIndex;
            sprite_->setGFXIndex(animations[currentAnimation][currentFrameIndex]);
            should_update_sprite_ = true;
        }
    }

    // Update the sprite if the actor is animating or has moved
    if (should_update_sprite_) {
        sprite_->update();
        should_update_sprite_ = false;
    }
}

void Actor::playAnimation(AnimationName name, bool loop) {
    if (currentAnimation == name)
        return;
    isAnimating_ = true;
    currentAnimation = name;
    loopAnimation = loop;
    animationTime = gba_milliseconds::zero();
    currentFrameIndex = 0;
    sprite_->setGFXIndex(animations[currentAnimation][currentFrameIndex]);
    should_update_sprite_ = true;
}

void Actor::stopAnimation() {
    isAnimating_ = false;
    loopAnimation = false;
    animationTime = gba_milliseconds::zero();
    currentFrameIndex = 0;
    stopAnimCallback();
}

bool Actor::isAnimating() const {
    return isAnimating_;
}
