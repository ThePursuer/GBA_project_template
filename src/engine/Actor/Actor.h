#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include <map>
#include <chrono>
#include <functional>

#include "engine/Graphics/Sprite.h"
#include "engine/Clock/GbaClock.h"

typedef u32 AnimationName;

class Actor {
public:
    // Constructor: takes a Sprite object as an argument
    Actor(Sprite* sprite);
    
    // Update the actor's state and animations
    virtual void update(gba_milliseconds deltaTime);

    // Set the actor's position
    void setPosition(s32 x, s32 y);

    // Get the actor's position
    s32 getX() const {return sprite_->getX();}
    s32 getY() const {return sprite_->getY();}
    
    // Add an animation sequence with an AnimationName, frame indices, and frame duration
    void addAnimation(AnimationName name, const std::vector<u32>& frameIndices, gba_milliseconds frameDuration);

    // Play the specified animation
    void playAnimation(AnimationName name, bool loop = true);

    // Stop the current animation
    void stopAnimation();

    // Check if the actor is currently animating
    bool isAnimating() const;

    void setStopAnimCallback(std::function<void()> cb){stopAnimCallback = cb;}

protected:
    Sprite* sprite_; // Pointer to the Sprite object representing this actor
    bool isAnimating_ = false;
    bool should_update_sprite_ = false;

    std::map<AnimationName, std::vector<u32>> animations; // Map of animation sequences
    std::map<AnimationName, gba_milliseconds> frameDurations; // Map of frame durations for each animation sequence
    AnimationName currentAnimation; // The current animation being played
    bool loopAnimation; // Whether the current animation should loop
    gba_milliseconds animationTime; // The time elapsed since the current animation started
    u32 currentFrameIndex; // The current frame index in the current animation sequence

    std::function<void()> stopAnimCallback  = []{return;};
};

#endif // ACTOR_H