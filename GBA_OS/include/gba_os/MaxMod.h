#ifndef GBA_OS_MAXMOD_H
#define GBA_OS_MAXMOD_H

#include <memory>

#include <maxmod.h>

#include "gba_os/Clock/GbaClock.h"
#include "gba_os/CoreDefs.h"
#include "gba_os/Core.h"

namespace gba_os::tasks::vblank {

void update_max_mod();

} // vblank

namespace gba_os::audio {
    
void mute();
void unMute();

} // audio

#endif // GBA_OS_MAXMOD_H