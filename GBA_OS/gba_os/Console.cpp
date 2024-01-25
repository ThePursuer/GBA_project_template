#include "gba_os/Console.h"

namespace gba_os::console {

void clearConsole() {
    printf("\x1b[2J"); // Clear the screen
    printf("\x1b[H");  // Move the cursor to the top-left corner
}

void moveCursor(int line, int column) {
    printf("\x1b[%d;%dH", line, column);
}

}