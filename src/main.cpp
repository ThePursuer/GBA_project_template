#include <gba.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include "engine/Input/InputHandler.h"

GbaClock::time_point start;

void showHelloWorld() {
    auto p = GbaClock::instance().now() - start;
    
    iprintf("%i\n", std::chrono::duration_cast<gba_milliseconds>(p).count());
}

int main() {
    // Set the timer frequency (16.38 kHz) and enable the timer
    REG_TM0CNT_H = TIMER_COUNT | (1 << 2) | TIMER_START;

    // Initialize GBA and devkitPro libraries
    irqInit();
    irqEnable(IRQ_VBLANK);
    consoleDemoInit();
    start = GbaClock::instance().now();

    
    // Initialize InputHandler
    InputHandler inputHandler;
    inputHandler.registerEvent(InputHandler::EventType::ButtonDoubleTap, KEY_A, showHelloWorld);
    
    // Main game loop
    while (1) {
        // Update InputHandler
        inputHandler.update();

        // Wait for VBlank to avoid screen tearing
        VBlankIntrWait();
    }

    return 0;
}
