#ifndef CORE_DEFS_H
#define CORE_DEFS_H
#include <functional>
#include <vector>
#include <cstdint>

namespace gba_os{

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
enum TASK_PRIORITY : uint16_t{
    HIGHEST = 100,
    HIGH,
    MEDIUM,
    LOW,
    LOWEST        
};

struct task{
    std::function<void(void*)> f;
    TASK_PRIORITY priority;
    void* data;
    int id = 0;
};

struct Callback {
    std::function<void(void*)> f;
    void* data;
    int id = 0;
};

}

#endif // CORE_DEFS_H