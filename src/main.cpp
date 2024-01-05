#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

#include <sstream>
#include <iostream>
#include <string>
#include <memory>

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "GBA_OS/Clock/GbaClock.h"

#include <libfixmath/fixmath.h>

void initializeGBA(){
    // Initialize GBA and devkitPro libraries
    irqInit();
	// irqSet(IRQ_VBLANK, mmVBlank );
    irqEnable(IRQ_VBLANK);

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
}

int main() {
    initializeGBA();

    GbaClock& clock = GbaClock::instance();

    volatile uint32_t count = 0;
    
    while (1) {
        if(count % 10 == 0){
            auto now = std::chrono::duration_cast<gba_milliseconds>(clock.now().time_since_epoch()).count();
            printf("\033[17;0HTIME: %i%i     ", now.hi, now.lo);
        }
        // Wait for VBlank to avoid screen tearing
        count++;
        VBlankIntrWait();
    }

    return 0;
}
