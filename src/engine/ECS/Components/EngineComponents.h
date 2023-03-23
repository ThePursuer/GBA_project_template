#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <vector>
#include <unordered_map>

#include <gba_types.h>

#include "engine/Graphics/Sprite.h"
#include "engine/Graphics/Animation.h"
#include "engine/Math/Collider.h"
#include "engine/Math/ColliderI.h"
#include "engine/Clock/GbaClock.h"
#include "engine/ECS/Component.h"
#include "engine/Signal.h"

#define COMPONENT_CAST(component) std::static_pointer_cast<Component>(component)

enum EngineReservedComponents : ComponentType{
        ANIMATION = 10000,
        COLLIDERS,
        POSITION,
        SPRITE
};

class AnimationComponent: public Component{
public:
    std::unordered_map<u32, Animation> sequences_;

    bool isAnimating_ = false;
    bool should_update_sprite_ = false;

    AnimationName currentAnimation = 0; // The current animation being played
    bool loopAnimation = false; // Whether the current animation should loop
    gba_milliseconds animationTime = gba_milliseconds(0); // The time elapsed since the current animation started
    u32 currentFrameIndex = 0; // The current frame index in the current animation sequence
    u32 lastFrameIndex = -1;
};

class SpriteComponent: public Component{
public:
    std::unique_ptr<Sprite> sprite;
    bool needs_update = true;
};

class ColliderComponent: public Component{
public:
    std::vector<Collider> colliders_;
};

class PositionComponent: Position, public Component{
public:
    const s32& getX() const {return x;}
    const s32& getY() const {return y;}
    s32 x, y;
};

#endif // ENGINE_COMPONENTS_H