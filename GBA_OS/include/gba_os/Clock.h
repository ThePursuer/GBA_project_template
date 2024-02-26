#ifndef GBA_CLOCK
#define GBA_CLOCK

#include <chrono>

#include "gba_timers.h"
#include "gba_interrupt.h"

namespace Gba_os::Clock {

class GbaClock {
// todo: implement time loading
public:
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<GbaClock> time_point;
    static const bool is_steady = true;

    static GbaClock& instance();

    std::chrono::time_point<GbaClock> now() noexcept;
private:

    GbaClock();

    static void irq_handler();
};

} // gba_os::clock

#endif // GBA_CLOCK