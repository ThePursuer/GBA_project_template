#include "gba_os/MaxMod.h"

static bool musicMuted = false;
static gba_os::chrono::GbaClock& clock_ = gba_os::chrono::GbaClock::instance();

namespace gba_os::tasks::vblank {

void update_max_mod(){
    mmVBlank();
    mmFrame();
}

} // vblank

namespace gba_os::tasks{

void fade_out_task(Task& task){
    if(musicMuted){
        gba_os::unregister_vblank_interrupt_task(task.id); // todo: test if this breaks
        return;
    }
    // fade_out_task_data& data = *static_cast<fade_out_task_data*>(task.data);
    // auto deltaTime = clock_.now();
    // if (data.fade_out_timer != gba_os::chrono::gba_milliseconds::zero()){
    //     mmSetModuleVolume(fadeOutTimer.count() / 3); // fade out is 3072ms which is 3x max volume.
    //     fadeOutTimer = (fadeOutTimer > deltaTime) ? 
    //         std::chrono::duration_cast<gba_os::chrono::gba_milliseconds>(fadeOutTimer -  deltaTime) : 
    //         gba_os::chrono::gba_milliseconds::zero();
    //     if (fadeOutTimer == gba_os::chrono::gba_milliseconds::zero()){
    //         mmStop();
    //         gba_os::unregister_vblank_interrupt_task(task.id); // todo: test if this breaks
    //         return;
    //     }
    // }
}

} // tasks

namespace gba_os::audio {

    
void mute(){
    mmSetModuleVolume(0);
    musicMuted = true;
}

void unMute(){
    mmSetModuleVolume(1024);
    musicMuted = false;
}

}