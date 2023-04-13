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

class AudioSystem: public ISystem {
public:

    void initialize(EntityManager& entityManager);
    void update(EntityManager& entityManager, gba_microseconds deltaTime);
    void shutdown(EntityManager& entityManager);
    std::unordered_set<ComponentType> requiredComponents() const;

    // To be used with Slots
    // Module slots
    void queueMusic(mm_word song, mm_pmode loop);
    void stopMusic();
    void fadeOut() {fadeOutTimer = gba_milliseconds(3072);}
    void mute();
    void unMute();

    // FX Slots
    void playFX(mm_sfxhand& handle, mm_word id);
    void cancelFX(mm_sfxhand handle);
private:
    std::queue<Song> songQueue;
    gba_milliseconds fadeOutTimer = gba_milliseconds::zero();
    bool musicMuted = false;
    bool fxMuted = false;

    std::shared_ptr<SoundFXComponent> soundFXComponent;
};