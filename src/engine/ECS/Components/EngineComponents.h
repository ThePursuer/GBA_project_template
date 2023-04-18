#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <vector>
#include <set>
#include <unordered_map>

#include <gba_types.h>

#include <maxmod.h>

#include "engine/Graphics/Sprite.h"
#include "engine/Graphics/Animation.h"
#include "engine/Math/Shape.h"
#include "engine/Clock/GbaClock.h"
#include "engine/ECS/Component.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Interface.h"
#include "engine/Math/RigidBody.h"
#include "engine/Math/Shape.h"

class AnimationClipComponent : public Component {
public:
    std::unordered_map<AnimationName, AnimationClip> clips;  // The animation clips for this entity
    AnimationName currentClip = -1;                           // The current animation clip being played
    u32 currentFrameIndex = 0;                           // The current frame index in the current animation clip
    gba_milliseconds elapsedTime = gba_milliseconds(0);  // The elapsed time since the current animation clip started
    bool loopAnimation = false;                          // Whether the current animation should loop
};

class SpriteComponent: public Component{
public:
    std::unique_ptr<Sprite> sprite;
    bool needs_update = true;
};

class PositionComponent: public Position, public Component {
public:
    std::shared_ptr<Vector2> position_; // optional variable for physics bodies
    bool changed;
};

class SoundbankComponent: public Component{
public:
    mm_addr addr;
    uint16_t channels;
};

class SoundFXComponent: public Component{
public:
    std::unordered_map<mm_word, mm_sound_effect> soundfx;
};

class SpacialComponent: public Component {
public:
    std::shared_ptr<gb_physics::Shape> shape;
    std::shared_ptr<gb_physics::Rigidbody> body;
};

#endif // ENGINE_COMPONENTS_H