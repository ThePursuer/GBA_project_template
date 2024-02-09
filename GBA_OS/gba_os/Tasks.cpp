#include "gba_os/Tasks.h"

namespace gba_os::tasks{

IWRAM_CODE void update_screen_mode_4_task(Task& task){
    void* screen_buffer = task.data;
    static int mode45_active_screen_buffer = 0; 
    // Set the display control and copy the screen data
    switch (mode45_active_screen_buffer) {
        case 0:
            DMA3COPY(screen_buffer, (void*)(VRAM), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (240 * 160 / 4));
            REG_DISPCNT &= ~((u16)BACKBUFFER); // Use front buffer
            break;
        case 1:
            DMA3COPY(screen_buffer, (void*)(VRAM + 0xA000), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (240 * 160 / 4));
            REG_DISPCNT |= (u16)BACKBUFFER;  // Use back buffer
            break;
    }
    mode45_active_screen_buffer = (mode45_active_screen_buffer + 1) % 2;
}

} // gba_os