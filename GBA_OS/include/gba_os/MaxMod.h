#ifndef GBA_OS_MAXMOD_H
#define GBA_OS_MAXMOD_H

#include <memory>

#include <maxmod.h>

#include "gba_os/CoreDefs.h"
#include "gba_os/Core.h"

namespace Gba_os::tasks::vblank {

void update_max_mod();

} // vblank

namespace Gba_os::audio {
    
void mute();
void unMute();

} // audio

#endif // GBA_OS_MAXMOD_H