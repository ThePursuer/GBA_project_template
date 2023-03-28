#pragma once

#include <vector>
#include <memory>

#include <gba_types.h>

#include "engine/Math/ColliderI.h"
#include "engine/Math/Vector.h"
#include "engine/Clock/GbaClock.h"

typedef s32 AnimationName;


struct AnimationClip {
    std::vector<u32> frames;
    std::shared_ptr<Collider> collider;
    gba_milliseconds clipDuration;
    const unsigned short* graphicsData;
    u32 graphicsDataLen;
    u32 frameCount;
    
    // Copy assignment operator
    AnimationClip& operator=(const AnimationClip& other) {
        frames = other.frames;                             // Copy the frames vector
        collider = other.collider;                         // Copy the shared_ptr, incrementing the reference count
        clipDuration = other.clipDuration;                 // Copy the duration
        graphicsData = other.graphicsData;                 // Copy the graphics data pointer
        graphicsDataLen = other.graphicsDataLen;           // Copy the graphics data length
        frameCount = other.frameCount;                     // Copy the frame count
        return *this;
    }
};
