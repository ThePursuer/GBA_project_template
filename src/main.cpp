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

#include <libfixmath/fixmath.h>

// #include <libfixmath/fixmath.h>

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
// -T gba_cart.ld%s
// rm -r build/ && /opt/devkitpro/portlibs/gba/bin/arm-none-eabi-cmake -B build -S . && make -C build
int main() {
    initializeGBA();

    while (1) {
		char * str = (char*)malloc(sizeof("Hello from IWRAM!"));
		memcpy(str, "Hello from IWRAM!", sizeof("Hello from IWRAM!"));

		fix16_t someint = fix16_from_int(10);
		auto res = fix16_add(someint, fix16_from_int(1));

		std::ostringstream address;
		address << "HERES THE THING: " << (void const *)str << " " << fix16_to_int(res);
		std::string name = address.str();
		memcpy((char*)0x03000500, name.c_str(), strlen(name.c_str()));
		free(str);
        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
