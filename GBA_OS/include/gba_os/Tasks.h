#ifndef TASKS_3DGBA_H
#define TASKS_3DGBA_H

#include <gba.h>

#include "gba_os/CoreDefs.h"

namespace Gba_os::tasks{

void update_screen_mode_4_task(Task& task);

struct Mode4Data {
    uint8_t** screen_buffer;
};

void switch_mode4_buffers(Task& data);

} // gba_os::tasks

#endif // TASKS_3DGBA_H
