#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include <vector>
#include <set>
#include <unordered_map>

#include <gba_types.h>

#include <maxmod.h>

#include <GBA_OS/Clock/GbaClock.h>

#include "ecs/Component.h"
#include "ecs/EntityManager.h"

class SoundbankComponent: public Component{
public:
    mm_addr addr;
    uint16_t channels;
};

class SoundFXComponent: public Component{
public:
    std::unordered_map<mm_word, mm_sound_effect> soundfx;
};

#endif // ENGINE_COMPONENTS_H