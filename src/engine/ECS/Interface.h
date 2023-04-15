#ifndef INTERFACE_H
#define INTERFACE_H

#include <gba_types.h>

class Position {
public:
    virtual const s32& getX() const = 0;
    virtual const s32& getY() const = 0;
};

#endif // INTERFACE_H