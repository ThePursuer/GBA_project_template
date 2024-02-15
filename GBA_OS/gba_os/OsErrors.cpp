#include "OsErrors.h"
#include "gba_os/Clock/GbaClock.h"

namespace gba_os::error {

using namespace gba_os::console;

void frame_duration_exceeded(void* data){
	chrono::gba_microseconds& delta = *static_cast<chrono::gba_microseconds*>(data);
    printf("Total task time: %lld\n", delta.count());
}

void invalid_pallet(void* data){
	printf("Pallet invalid: %p", data);
}

void software_runtime_error(void* data){
	std::string& str = *static_cast<std::string*>(data);
	printf("Software runtime error:\n");
	printf("%s", str.c_str());
}

void error_state(os_error err, void* data){
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
	clearConsole();
    
    printf("ERROR: %i", err);
	moveCursor(1, 0);

	switch (err)
	{
	case os_error::FRAME_DURATION_EXCEEDED:
		frame_duration_exceeded(data);
		break;
	case os_error::INVALID_PALLET:
		invalid_pallet(data);
		break;
	case os_error::SFOTWARE_RUNTIME_ERROR:
		software_runtime_error(data);
		break;
	default:
		break;
	}
    while(1);
}

}
