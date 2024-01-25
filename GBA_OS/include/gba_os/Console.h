#ifndef GBA_OS_CONSOLE_H
#define GBA_OS_CONSOLE_H

#include <cstdio>

namespace gba_os::console {

// Function to clear the console
void clearConsole();

// Function to move the cursor to a specific line and column
void moveCursor(int line, int column);

}

#endif // GBA_OS_CONSOLE_H