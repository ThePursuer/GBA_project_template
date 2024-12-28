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
#include <cstdlib>
#include <cmath>

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include <gba_os/Core.h>
#include <gba_os/Screen.h>
#include <gba_os/Console.h>
#include <gba_os/UnitTest.h>
#include <gba_os/Tasks.h>
#include <gba_os/QuickTimer.h>
#include <gba_os/Clock.h>

#include <3Dgba/ui_overlay.h>
#include <3Dgba/rasterize.h>
#include <3Dgba/amiga_fnt.h>

#define PALETTE_SIZE 256

using namespace Gba_os::console;
using namespace Gba_os::Clock;

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

// Function to generate a random number between min and max (inclusive)
IWRAM_CODE int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to fill the array with random Vector2s_t values
IWRAM_CODE void fillArrayWithRandom(Vector2i_t* array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i].x = getRandomNumber(0, int_to_fix14(240));
        array[i].y = getRandomNumber(0, int_to_fix14(160));
    }
}

struct draw_triangles_data_s{
    Vector2i_t* array;
    int arraySize;
    uint8_t** screen_buffer;
};

const int arraySize = 99; // Change this to the desired size of the array
EWRAM_DATA Vector2i_t array[arraySize];

IWRAM_CODE void draw_one_triangle_task(Gba_os::Task& t){
    draw_triangles_data_s &data = *reinterpret_cast<draw_triangles_data_s*>(t.data);

    Vector2i_t tri[3] = {
        Vector2i_t{int_to_fix14(150), int_to_fix14(100)}, 
        Vector2i_t{int_to_fix14(20), int_to_fix14(10)}, 
        Vector2i_t{int_to_fix14(30), int_to_fix14(10)}};
    static int color = 0x14;
    for(int i = 0; i < 3; i+=3) {
        // Vector2i_t (&tri)[3] = *reinterpret_cast<Vector2i_t(*)[3]>(&data.array[i]);
        gba_3D::drawTriangle(*data.screen_buffer, tri, color);
    }
}

IWRAM_CODE void draw_triangles_task(Gba_os::Task& t){
    draw_triangles_data_s &data = *reinterpret_cast<draw_triangles_data_s*>(t.data);
    fillArrayWithRandom(array, arraySize);

    static int color = 0x14;
    for(int i = 0; i < arraySize; i+=3) {
        Vector2i_t (&tri)[3] = *reinterpret_cast<Vector2i_t(*)[3]>(&data.array[i]);
        gba_3D::drawTriangle(*data.screen_buffer, tri, color);
        color = (color + 1) % 255;
    }
}

int main() {
    static uint16_t * pallet;
    static uint8_t * screen_buffer;
    screen_buffer = new uint8_t[240 * 160];
    pallet = new uint16_t[256];
    srand(34); // seed the random number generator
    fillPaletteBuffer(pallet);
    set_font(amiga_fnt, amiga_fnt_size);

    Gba_os::init_gba_os();

    Gba_os::tasks::Mode4Data mode4_data;
    mode4_data.screen_buffer = new uint8_t*();

    consoleDemoInit();

    GbaClock& my_clock = GbaClock::instance();

    // gba_os::Task mode4;
    // mode4.f = &gba_os::tasks::switch_mode4_buffers;
    // mode4.data = &mode4_data; // the buffer for the screen needs to be assigned here
    // mode4.priority = (gba_os::TASK_PRIORITY)0;
    // int mode4_task_id = gba_os::register_task(mode4);

    draw_triangles_data_s draw_triangles_data;
    draw_triangles_data.array = array;
    draw_triangles_data.arraySize = arraySize;
    draw_triangles_data.screen_buffer = new uint8_t*();
    *draw_triangles_data.screen_buffer = (uint8_t*)(VRAM); 

    Gba_os::Task draw_triangles;
    // draw_triangles.f = &draw_one_triangle_task;
    draw_triangles.f = &draw_triangles_task;
    draw_triangles.data = &draw_triangles_data;
    draw_triangles.priority = Gba_os::TASK_PRIORITY::HIGH;
    int draw_triangles_task_id = Gba_os::register_task(draw_triangles);

    Gba_os::screen::set_palette_mode_4(pallet);
    Gba_os::screen::set_user_mode(MODE_4 | BG2_ENABLE);
    Gba_os::set_framerate(Gba_os::video::FRAMERATE30);  

    Gba_os::waitNextFrame();
    while (1) {
        Gba_os::tick();
    }
    return 0;
}
