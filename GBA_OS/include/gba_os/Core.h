#ifndef GBA_CORE_H
#define GBA_CORE_H
#include <cstdio>
#include <unordered_set>

#include <gba.h>

#include "gba_os/CoreDefs.h"
#include "gba_os/Clock/GbaClock.h"

namespace gba_os{
    void init_gba_os();
    void run_gba_os();

    void set_framerate(video::FRAMERATE framerate);

    int register_task(Task t);
    void unregister_task(int id);

    int register_vblank_interrupt_task(Task task);
    void unregister_vblank_interrupt_task(int id);
}

#endif // GBA_CORE_H