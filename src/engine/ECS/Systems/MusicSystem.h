#pragma once

#include <queue>

#include "engine/Clock/GbaClock.h"

#include "engine/ECS/System.h"
#include "engine/ECS/Components/EngineComponents.h"

struct Song
{
    mm_word song;
    mm_pmode mode;
};

class MusicSystem: public ISystem {
public:

    void initialize(EntityManager& entityManager);
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

    // To be used with Slots
    void queueMusic(mm_word song, mm_pmode loop);
    void stopMusic();
    void fadeOut() {fadeOutTimer = gba_milliseconds(3072);}
    void mute();
    void unMute();
private:
    std::queue<Song> songQueue;
    gba_milliseconds fadeOutTimer = gba_milliseconds::zero();
    bool muted = false;
};