#ifndef SYSTEM_COMPONENTS_H
#define SYSTEM_COMPONENTS_H

#include <vector>
#include <map>

#include <gba_types.h>

#include "engine/Graphics/Sprite.h"
#include "engine/Graphics/Animation.h"
#include "engine/Math/Collider.h"
#include "engine/Math/ColliderI.h"
#include "engine/Clock/GbaClock.h"
#include "engine/ECS/Component.h"

#define COMPONENT_CAST(component) std::static_pointer_cast<Component>(component)

enum EngineReservedComponents : ComponentType{
        ANIMATION = 10000,
        COLLIDERS,
        POSITION,
        SPRITE
};

auto wow = EngineReservedComponents::ANIMATION;

class AnimationComponent: Component{
public:
    std::map<u32, Animation> sequences_;

    bool isAnimating_ = false;
    bool should_update_sprite_ = false;
    bool isUpdating_;

    AnimationName currentAnimation; // The current animation being played
    bool loopAnimation; // Whether the current animation should loop
    gba_milliseconds animationTime; // The time elapsed since the current animation started
    u32 currentFrameIndex; // The current frame index in the current animation sequence
};

class SpriteComponent: Component{
public:
    std::unique_ptr<Sprite> sprite;
};

class ColliderComponent: Component{
public:
    std::vector<Collider> colliders_;
};

class PositionComponent: Position, Component{
public:
    const u32& getX() const {return x;}
    const u32& getY() const {return y;}
    u32 x, y;
};

#endif // SYSTEM_COMPONENTS_H