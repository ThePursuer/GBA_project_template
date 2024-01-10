#ifndef GBA_CORE_H
#define GBA_CORE_H
#include <stdio.h>

#include <gba.h>

#include "CoreDefs.h"
#include "Clock/GbaClock.h"

namespace gba_os{
    void init_gba_os();
    void run_gba_os();

    void set_framerate(video::FRAMERATE framerate);

    int register_task(task t);
    void unregister_task(int id);

    int register_vblank_interrupt_callback(Callback cb);
    void unregister_vblank_interrupt_callback(int id);

}

#endif // GBA_CORE_H