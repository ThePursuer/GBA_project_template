#ifndef INTERFACE_H
#define INTERFACE_H

#include <gba_types.h>

class Position {
public:
    virtual const __attribute__((section(".iwram"), long_call)) s32& getX() const = 0;
    virtual const __attribute__((section(".iwram"), long_call)) s32& getY() const = 0;
};

#endif // INTERFACE_H