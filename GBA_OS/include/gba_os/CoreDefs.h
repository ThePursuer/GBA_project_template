#ifndef CORE_DEFS_H
#define CORE_DEFS_H
#include <functional>
#include <vector>
#include <cstdint>

namespace Gba_os{

namespace video{

enum FRAMERATE {
    FRAMERATE60,
    FRAMERATE30,
    FRAMERATE15
};

} // video

// priorities 0-99 can be used to order tasks in a specific order.
// For example, updating the screen must occur during the begining of vblank to avoid screen tearing.
// Tasks that do not need a specific order should use these and will run last
enum TASK_PRIORITY : uint8_t{
    HIGHEST = 100,
    HIGH,
    MEDIUM,
    LOW,
    LOWEST        
};

struct Task{
    void(*f)(Task&);
    void* data = nullptr;
    TASK_PRIORITY priority = LOWEST;
    uint16_t id = 0;
};

}

#endif // CORE_DEFS_H