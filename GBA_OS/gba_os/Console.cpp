#include "gba_os/Console.h"

namespace gba_os::console {

void clearConsole() {
    printf("\x1b[2J"); // Clear the screen
    printf("\x1b[H");  // Move the cursor to the top-left corner
}

void moveCursor(int line, int column) {
    printf("\x1b[%d;%dH", line, column);
}

void initialize_console() {
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

}