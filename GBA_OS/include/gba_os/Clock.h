#ifndef GBA_CLOCK
#define GBA_CLOCK

#include <chrono>

#include "gba_timers.h"
#include "gba_interrupt.h"

namespace Gba_os::Clock {

class GbaClock {
// todo: implement time loading from save file
public:
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<GbaClock> time_point;
    static const bool is_steady = true;

    static GbaClock& instance();

    static void irq_handler();

    std::chrono::time_point<GbaClock> now() noexcept;
private:

    GbaClock();

};

} // gba_os::clock

#endif // GBA_CLOCK