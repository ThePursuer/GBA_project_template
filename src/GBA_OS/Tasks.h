#ifndef GBA_TASKS_H
#define GBA_TASKS_H
/**
 * \brief This file contains builtin tasks for the os 
 */

#include "CoreDefs.h"

#include "gba_dma.h"
#include "gba_video.h"

namespace gba_os {
namespace builtin {

void update_screen_mode_4_task(void * screen_buffer);

} // builtin
} // gba_os

#endif // GBA_TASKS_H