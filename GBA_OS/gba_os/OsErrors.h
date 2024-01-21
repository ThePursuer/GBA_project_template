#ifndef OS_ERRORS_H
#define OS_ERRORS_H

#include <cstdio>

#include <gba.h>

namespace gba_os::error {

enum os_error {
    FRAME_DURATION_EXCEEDED = 1
};

void error_state(os_error err, void* data);

}

#endif // OS_ERRORS_H