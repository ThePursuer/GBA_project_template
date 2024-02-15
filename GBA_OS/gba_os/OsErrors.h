#ifndef OS_ERRORS_H
#define OS_ERRORS_H

#include <cstdio>
#include <string>

#include <gba.h>

#include "gba_os/Console.h"

namespace gba_os::error {

enum os_error {
    FRAME_DURATION_EXCEEDED,
    INVALID_PALLET,
    SFOTWARE_RUNTIME_ERROR
};

void error_state(os_error err, void* data);

}

#endif // OS_ERRORS_H