#include "ecs/Systems/AudioSystem.h"

void AudioSystem::initialize(EntityManager& entityManager){
    Entity soundbankEntity = entityManager.getEntitiesWithComponent(EngineReservedComponents::SOUNDBANK)[0];
    auto soundbankComponent = std::static_pointer_cast<SoundbankComponent>(entityManager.getComponent(soundbankEntity, EngineReservedComponents::SOUNDBANK));
    
    Entity soundbankFX = entityManager.getEntitiesWithComponent(EngineReservedComponents::SOUND_EFFECT)[0];
    soundFXComponent = std::static_pointer_cast<SoundFXComponent>(entityManager.getComponent(soundbankFX, EngineReservedComponents::SOUND_EFFECT));

	// initialise maxmod with soundbank and 8 channels
    mmInitDefault(soundbankComponent->addr, soundbankComponent->channels);
}

void AudioSystem::update(EntityManager& entityManager, gba_os::chrono::gba_microseconds deltaTime){
    if(!musicMuted){
        if (fadeOutTimer != gba_os::chrono::gba_milliseconds::zero()){
            mmSetModuleVolume(fadeOutTimer.count() / 3); // fade out is 3072ms which is 3x max volume.
            fadeOutTimer = (fadeOutTimer > deltaTime) ? 
                std::chrono::duration_cast<gba_os::chrono::gba_milliseconds>(fadeOutTimer -  deltaTime) : 
                gba_os::chrono::gba_milliseconds::zero();
            if (fadeOutTimer == gba_os::chrono::gba_milliseconds::zero()){
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

void AudioSystem::shutdown(EntityManager& entityManager){
    stopMusic();
}

std::unordered_set<ComponentType> AudioSystem::requiredComponents() const {
    return {EngineReservedComponents::SOUNDBANK};
}

void AudioSystem::queueMusic(mm_word song, mm_pmode mode){
    if (!mmActive())
        mmStart(song, mode);
    else
        songQueue.push({song, mode});
}

void AudioSystem::stopMusic(){
    while(songQueue.size() != 0)
        songQueue.pop();
    mmStop();
}

void AudioSystem::mute(){
    mmSetModuleVolume(0);
    musicMuted = true;
}

void AudioSystem::unMute(){
    mmSetModuleVolume(1024);
    musicMuted = false;
}

void AudioSystem::playFX(mm_sfxhand& handle, mm_word id){
    auto fx = soundFXComponent->soundfx[id];
    handle = mmEffectEx(&fx);
}

void AudioSystem::cancelFX(mm_sfxhand handle){
    mmEffectCancel(handle);
}
