#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <gba_base.h>

#include <sstream>
#include <iostream>
#include <string>
#include <memory>

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "GBA_OS/Clock/GbaClock.h"
#include "GBA_OS/Core.h"
#include "GBA_OS/Tasks.h"
#include "GBA_OS/Screen.h"

#define PALETTE_SIZE 256

// Function to create a 16-bit color from red, green, and blue components
u16 createColor(u8 red, u8 green, u8 blue) {
    return (red & 0x1F) | ((green & 0x1F) << 5) | ((blue & 0x1F) << 10);
}

// Function to fill the palette buffer with a spread of colors
void fillPaletteBuffer(u16* paletteBuffer) {
    for (int i = 0; i < PALETTE_SIZE; ++i) {
        u8 red = (i * 7) % 32;   // Cycle through red values
        u8 green = (i * 13) % 32; // Cycle through green values
        u8 blue = (i * 19) % 32;  // Cycle through blue values

        paletteBuffer[i] = createColor(red, green, blue);
    }
}

EWRAM_DATA uint8_t screen_buffer[160][240];

int main() {
    uint16_t * pallet = new uint16_t[256];
    fillPaletteBuffer(pallet);

    gba_os::init_gba_os();

    gba_os::task mode4;
    mode4.f = &gba_os::builtin::update_screen_mode_4_task;
    mode4.data = (void*)&screen_buffer;
    mode4.priority = (gba_os::TASK_PRIORITY)0;
    int mode4_task_id = gba_os::register_task(mode4);

    gba_os::screen::set_palette_mode_4(pallet);
    gba_os::screen::set_mode4();
    gba_os::set_framerate(gba_os::video::FRAMERATE60);
    
    gba_os::run_gba_os();

    return 0;
}
