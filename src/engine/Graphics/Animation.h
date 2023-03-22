#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <memory>

#include <gba_types.h>

#include "engine/Math/ColliderI.h"
#include "engine/Math/Vector.h"
#include "engine/Clock/GbaClock.h"

typedef u32 AnimationName;

struct Animation {
    Animation() = default; // default constructor
    Animation(const std::vector<u32> frames, const gba_microseconds frameDuration): frames_(frames), frameDuration_(frameDuration){}
    Animation operator=(const Animation& animation){return Animation(animation.frames_, animation.frameDuration_);}

    std::vector<u32> frames_;
    gba_microseconds frameDuration_;
};

#endif