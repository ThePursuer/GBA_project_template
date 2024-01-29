#ifndef GBA_CLOCK
#define GBA_CLOCK

#include <chrono>

#include "gba_timers.h"
#include "gba_interrupt.h"
//todo: Fix a bug where time_point + duration doesnst work 

namespace gba_os {
namespace chrono {

// Equvalents of std::chrono::<resolution>, required because std::chrono uses 64bit integers and we don't have those. 
typedef std::chrono::nanoseconds gba_nanoseconds;
typedef std::chrono::microseconds gba_microseconds;
typedef std::chrono::milliseconds gba_milliseconds;
typedef std::chrono::seconds gba_seconds;

class GbaClock {
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

} // chrono
} // gba_os

#endif // GBA_CLOCK