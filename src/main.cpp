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

#include <gba_os/Clock/GbaClock.h>
#include <gba_os/Core.h>
#include <gba_os/Tasks.h>
#include <gba_os/Screen.h>

#include "untitled_obj.h"

#include <libfixmath/fixmath.h>

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

void run_mode_4_demo(){
    uint16_t * pallet = new uint16_t[256];
    fillPaletteBuffer(pallet);

    gba_os::init_gba_os();

    gba_os::Task mode4;
    mode4.f = &gba_os::tasks::update_screen_mode_4_task;
    mode4.data = (void*)&screen_buffer;
    mode4.priority = (gba_os::TASK_PRIORITY)0;
    int mode4_task_id = gba_os::register_task(mode4);

    gba_os::screen::set_palette_mode_4(pallet);
    gba_os::screen::set_mode4();
    gba_os::set_framerate(gba_os::video::FRAMERATE60);
    
    gba_os::run_gba_os();
}

#include <gba.h>

// Start the timer
void startTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value

    // Set control to start the timer with a 1:1 prescaler and enable interrupts if needed
    REG_TM0CNT_H = TIMER_START;
}

// Stop the timer and return elapsed time in clock cycles
u16 stopTimer() {
    // Disable the timer
    REG_TM0CNT_H &= ~TIMER_START;
    
    // Return the value of the timer
    return REG_TM0CNT_L;
}

// Reset the timer
void resetTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value
}

// Function to clear the console
void clearConsole() {
    printf("\x1b[2J"); // Clear the screen
    printf("\x1b[H");  // Move the cursor to the top-left corner
}

// Function to move the cursor to a specific line and column
void moveCursor(int line, int column) {
    printf("\x1b[%d;%dH", line, column);
}


__attribute__((optimize(0))) fix16_t divide(fix16_t a, fix16_t b){
    return fix16_div(a, b);
}

__attribute__((optimize(0))) float dividef(float a, float b){
    return a / b;
}


void run_console_test(){
    // initialise the console
	// setting NULL & 0 for the font address & size uses the default font
	// The font should be a complete 1bit 8x8 ASCII font
	consoleInit(	0,		// charbase
					4,		// mapbase
					0,		// background number
					NULL,	// font
					0, 		// font size
					15		// 16 color palette
	);

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0]=RGB8(58,110,165);
	BG_COLORS[241]=RGB5(31,31,31);

    // Set up the video mode and enable sprites
    SetMode(MODE_0 | BG0_ON | OBJ_ENABLE | OBJ_1D_MAP);

    volatile float numf = 33;
    volatile float denomf = 16;

    volatile fix16_t numerator = fix16_from_int(33);
    volatile fix16_t denominator = fix16_from_int(16);

    clearConsole();

    // Test libfixmath and print result
    startTimer();
    auto res = divide(numerator, denominator);
    auto delta = stopTimer();
    printf("FMelapsed: %i, result: %i", delta, res);
    moveCursor(2,0);
    resetTimer();

    
    // test reg registers and result
    startTimer();
    auto resf = dividef(numf, denomf);
    delta = stopTimer();
    printf("FOPelapsed: %i, result: %f", delta, resf);
    resetTimer();

    while(1){VBlankIntrWait();};
}

int main() {
    run_console_test();

    return 0;
}
