#include "engine/ECS/Systems/MusicSystem.h"

void MusicSystem::initialize(EntityManager& entityManager){
    Entity soundbankEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::SOUNDBANK)[0];
    auto soundbankComponent = std::static_pointer_cast<SoundbankComponent>(entityManager.getComponent(soundbankEntity, EngineReservedComponents::SOUNDBANK));
    
	// initialise maxmod with soundbank and 8 channels
    mmInitDefault(soundbankComponent->addr, soundbankComponent->channels);
}

void MusicSystem::update(EntityManager& entityManager, gba_microseconds deltaTime){
    if(!muted){
        if (fadeOutTimer != gba_milliseconds::zero()){
            mmSetModuleVolume(fadeOutTimer.count() / 3); // fade out is 3072ms which is 3x max volume.
            fadeOutTimer = (fadeOutTimer > deltaTime) ? std::chrono::duration_cast<gba_milliseconds>(fadeOutTimer -  deltaTime) : gba_milliseconds::zero();
            if (fadeOutTimer == gba_milliseconds::zero()){
                mmStop();
                if(songQueue.size() != 0){
                    Song queuedSong = songQueue.front();
                    mmStart(queuedSong.song, queuedSong.mode);
                    songQueue.pop();
                }
            }
        }
    }
    mmFrame();
}

void MusicSystem::shutdown(EntityManager& entityManager){
    stopMusic();
}

std::unordered_set<ComponentType> MusicSystem::requiredComponents() const {
    return {EngineReservedComponents::SOUNDBANK};
}

void MusicSystem::queueMusic(mm_word song, mm_pmode mode){
    if (!mmActive())
        mmStart(song, mode);
    else
        songQueue.push({song, mode});
}

void MusicSystem::stopMusic(){
    while(songQueue.size() != 0)
        songQueue.pop();
    mmStop();
}

void MusicSystem::mute(){
    mmSetModuleVolume(0);
    muted = true;
}

void MusicSystem::unMute(){
    mmSetModuleVolume(1024);
    muted = false;
}
