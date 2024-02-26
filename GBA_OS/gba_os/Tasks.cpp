#include "gba_os/Tasks.h"

namespace Gba_os::tasks{

EWRAM_CODE void update_screen_mode_4_task(Task& task){
    void* screen_buffer = task.data;
    static volatile int mode45_active_screen_buffer = 0; 
    // Set the display control and copy the screen data
    switch (mode45_active_screen_buffer) {
        case 0:
            DMA3COPY(screen_buffer, (void*)(VRAM), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_VBLANK | (240 * 160) >> 2);
            REG_DISPCNT &= ~((u16)BACKBUFFER); // Use front buffer
            break;
        case 1:
            DMA3COPY(screen_buffer, (void*)(VRAM + 0xA000), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_VBLANK | (240 * 160) >> 2);
            REG_DISPCNT |= (u16)BACKBUFFER;  // Use back buffer
            break;
    }
    mode45_active_screen_buffer = (mode45_active_screen_buffer + 1) % 2;
}

static uint8_t* mode4_screen_buffer0 = (uint8_t*)VRAM;
static uint8_t* mode4_screen_buffer1 = (uint8_t*)(VRAM + 0xA000);

void switch_mode4_buffers(Task& data){
    // switches the buffers and updates the task data pointer to the new buffer
    static volatile int mode45_active_screen_buffer = 0;
    switch (mode45_active_screen_buffer) {
        case 0:
            *((Mode4Data*)(data.data))->screen_buffer = mode4_screen_buffer1;
            REG_DISPCNT |= (u16)BACKBUFFER;  // Use back buffer
            break;
        case 1:
            *((Mode4Data*)(data.data))->screen_buffer = mode4_screen_buffer0;
            REG_DISPCNT &= ~((u16)BACKBUFFER); // Use front buffer
            break;
    }
    mode45_active_screen_buffer = (mode45_active_screen_buffer + 1) % 2;
}

} // gba_os