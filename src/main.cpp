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

#include <3Dgba/ui_overlay.h>
#include <3Dgba/rasterize.h>
#include <3Dgba/amiga_fnt.h>

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

// Function to generate a random number between min and max (inclusive)
IWRAM_CODE int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to fill the array with random Vector2s_t values
IWRAM_CODE void fillArrayWithRandom(Vector2s_t* array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i].x = getRandomNumber(0, int_to_fix7(240));
        array[i].y = getRandomNumber(0, int_to_fix7(160));
    }
}

struct draw_triangles_data_s{
    Vector2s_t* array;
    int arraySize;
    uint8_t* screen_buffer;
};

const int arraySize = 12; // Change this to the desired size of the array
Vector2s_t array[arraySize];

IWRAM_CODE void draw_triangles_task(gba_os::Task& t){
    static int i = 0;
    draw_triangles_data_s &data = *reinterpret_cast<draw_triangles_data_s*>(t.data);
    fillArrayWithRandom(array, arraySize);

    resetTimer();
    startTimer();
    for(int i = 0; i < data.arraySize; i+=3)
        gba_3D::drawTriangle(*reinterpret_cast<Vector2s_t(*)[3]>(&data.array[i]), data.screen_buffer);
    auto stop_time = stopTimer();
    print_text(data.screen_buffer, 10, 20, i, "Time: %i", stop_time);
    i = (i + 1) % 256;
}

void run_mode_4_demo(){
    srand(34); // seed the random number generator
    uint16_t * pallet = new uint16_t[256];
    fillPaletteBuffer(pallet);
    set_font(amiga_fnt, amiga_fnt_size);

    gba_os::init_gba_os();
    uint8_t * screen_buffer = new uint8_t[240 * 160];

    gba_os::Task mode4;
    mode4.f = &gba_os::tasks::update_screen_mode_4_task;
    mode4.data = screen_buffer; // the buffer for the screen needs to be assigned here
    mode4.priority = (gba_os::TASK_PRIORITY)0;
    int mode4_task_id = gba_os::register_task(mode4);
    
    draw_triangles_data_s draw_triangles_data;
    draw_triangles_data.array = array;
    draw_triangles_data.arraySize = arraySize;
    draw_triangles_data.screen_buffer = screen_buffer; 

    gba_os::Task draw_triangles;
    draw_triangles.f = &draw_triangles_task;
    draw_triangles.data = &draw_triangles_data;
    draw_triangles.priority = gba_os::TASK_PRIORITY::HIGH;
    int draw_triangles_task_id = gba_os::register_task(draw_triangles);

    gba_os::screen::set_palette_mode_4(pallet);
    gba_os::screen::set_mode4();
    gba_os::set_framerate(gba_os::video::FRAMERATE30);
    
    gba_os::run_gba_os();   
}

int main() {
    run_mode_4_demo();
    return 0;
}
