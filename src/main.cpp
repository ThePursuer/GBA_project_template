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
#include <cmath>

#include <gba_os/Clock/GbaClock.h>
#include <gba_os/Core.h>
#include <gba_os/Screen.h>
#include <gba_os/Console.h>
#include <gba_os/UnitTest.h>
#include <gba_os/Tasks.h>
#include <gba_os/QuickTimer.h>

#include <3Dgba/rasterize.h>

#define PALETTE_SIZE 256

using namespace gba_os::console;

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

// void drawthing(void* fb){
//     for (int i = 10; i < 100; i++){
//         for (int j = 10; j < 100; j++){
//             ((uint8_t*)fb)[i * 240 + j] = 0x14;
//         }
//     }
// }

void run_mode_4_demo(){
    uint16_t * pallet = new uint16_t[256];
    fillPaletteBuffer(pallet);

    gba_os::init_gba_os();
    uint8_t * screen_buffer = new uint8_t[240 * 160];

    gba_os::Task mode4;
    mode4.f = &gba_os::tasks::update_screen_mode_4_task;
    mode4.data = screen_buffer; // the buffer for the screen needs to be assigned here
    mode4.priority = (gba_os::TASK_PRIORITY)0;
    int mode4_task_id = gba_os::register_task(mode4);

    Vector2c_t triangle[3] = {
        { 50, 10 },
        { 10, 80 },
        { 80, 20 }
    };

    resetTimer();
    startTimer();
    gba_3D::drawTriangle(triangle, screen_buffer);
    auto stop_time = stopTimer();
    // gba_os::console::initialize_console();
    // gba_os::console::moveCursor(0, 0);
    // printf("Time to draw triangle: %d\n", stop_time);
    // while(1){} // infinite loop to keep the program running

    // drawthing(screen_buffer);
    gba_os::screen::set_palette_mode_4(pallet);
    gba_os::screen::set_mode4();
    gba_os::set_framerate(gba_os::video::FRAMERATE30);
    
    gba_os::run_gba_os();   
}

int main() {
    run_mode_4_demo();
    return 0;
}
