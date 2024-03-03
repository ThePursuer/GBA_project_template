#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <vector>
#include <set>
#include <unordered_map>

#include <gba_types.h>

#include <maxmod.h>

#include <chipmunk/chipmunk_structs.h>

#include "engine/Graphics/Sprite.h"
#include "engine/Graphics/Animation.h"
#include "engine/Clock/GbaClock.h"
#include "engine/ECS/Component.h"
#include "engine/ECS/EntityManager.h"
#include "engine/ECS/Interface.h"

struct PhysicsStructure {
    std::shared_ptr<cpBody> body;
    std::shared_ptr<cpShape> shape;
};

typedef u16 FormName;
class PhysicsComponent: public Component {
public:
    std::unordered_map<FormName, PhysicsStructure> forms;
    FormName currentForm;
};

class AnimationClipComponent: public Component{
public:
    std::unordered_map<AnimationName, AnimationClip> clips;  // The animation clips for this entity
    AnimationName currentClip = -1;                           // The current animation clip being played
    u32 currentFrameIndex = 0;                           // The current frame index in the current animation clip
    gba_milliseconds elapsedTime = gba_milliseconds(0);  // The elapsed time since the current animation clip started
    bool loopAnimation = false;                          // Whether the current animation should loop
};

class SpriteComponent: public Component {
public:
    std::unique_ptr<SpriteMine> sprite;
    bool needs_update = true;
};

class PositionComponent: public Position, public Component {
public:
    const s32& getX() const {return x;}
    const s32& getY() const {return y;}
    const float& getSuperPositionX() const {return superPositionX;}
    const float& getSuperPositionY() const {return superPositionY;}
    float superPositionX, superPositionY; // Useful for modifying position by half pixels accross frames. Helps reduce physics bugs.
    s32 x, y;
};

class SoundbankComponent: public Component {
public:
    mm_addr addr;
    uint16_t channels;
};

class SoundFXComponent: public Component {
public:
    std::unordered_map<mm_word, mm_sound_effect> soundfx;
};

#endif // ENGINE_COMPONENTS_H