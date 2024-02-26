#ifndef GBA_CORE_H
#define GBA_CORE_H
#include <cstdio>
#include <unordered_set>
#include <string>

#include <gba.h>

#include "gba_os/CoreDefs.h"
#include "gba_os/Screen.h"

namespace Gba_os{
    void init_gba_os();
    void waitNextFrame(); // waits for the next frame to start note: Call this before the first tick()!!
    void tick();
    void raise_software_error(std::string error); // halts execution and displays error message

    void set_framerate(video::FRAMERATE framerate);

    int register_task(Task t);
    void unregister_task(int id);

    int register_vblank_interrupt_task(Task task);
    void unregister_vblank_interrupt_task(int id);

    void alert(std::string alert_message); // changes to screen mode 0 and displays the alert message, restores user screen mode and returns after pressing a button
}

#endif // GBA_CORE_H