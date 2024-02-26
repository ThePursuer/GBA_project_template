#ifndef OS_ERRORS_H
#define OS_ERRORS_H

#include <cstdio>
#include <string>
#include <functional>

#include <gba.h>

#include "gba_os/Console.h"
#include "gba_os/Screen.h"
#include "gba_os/Clock.h"

namespace Gba_os::error {

enum os_error {
    FRAME_DURATION_EXCEEDED,
    INVALID_PALLET,
    SFOTWARE_RUNTIME_ERROR
};

void error_state(os_error err, void* data); // sets the screen mode to 0 and calls the error handler, restores the user screen mode after

}

#endif // OS_ERRORS_H