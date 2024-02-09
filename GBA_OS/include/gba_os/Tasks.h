#ifndef TASKS_3DGBA_H
#define TASKS_3DGBA_H

#include <gba.h>

#include "gba_os/CoreDefs.h"

namespace gba_os::tasks{

IWRAM_CODE void update_screen_mode_4_task(Task& task);

} // gba_os

#endif // TASKS_3DGBA_H
