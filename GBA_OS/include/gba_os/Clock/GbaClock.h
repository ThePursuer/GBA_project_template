#ifndef GBA_CLOCK
#define GBA_CLOCK

#include <chrono>

#include "gba_timers.h"
#include "gba_interrupt.h"
//todo: Fix a bug where time_point + duration doesnst work 

namespace gba_os {
namespace chrono {

class GbaClock {
public:
    typedef std::chrono::milliseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<GbaClock> time_point;
    static const bool is_steady = true;

    static GbaClock& instance();

    std::chrono::time_point<GbaClock> now() noexcept;

    void handle_interrupt();

private:
    u32 rollovers;

    GbaClock();

    static void irq_handler();
};

} // chrono
} // gba_os

#endif // GBA_CLOCK