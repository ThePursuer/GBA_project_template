#ifndef GBA_OS_CONSOLE_H
#define GBA_OS_CONSOLE_H

#include <cstdio>

#include <gba_console.h>
#include <gba_video.h>

namespace gba_os::console {

// Function to clear the console
void clearConsole();

// Function to move the cursor to a specific line and column
void moveCursor(int line, int column);

// Function to initialize the console
void initialize_console();

}

#endif // GBA_OS_CONSOLE_H