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
    Animation(const std::vector<u32> frames, const gba_milliseconds frameDuration): frames_(frames), frameDuration_(frameDuration){}
    Animation operator=(const Animation& animation){
        frames_ = animation.frames_;
        frameDuration_ = animation.frameDuration_;
        return *this;
    }

    std::vector<u32> frames_;
    gba_milliseconds frameDuration_;
};

#endif