#ifndef GBA_CLOCK
#define GBA_CLOCK

#include <chrono>
#include "gba_timers.h"
#include "gba_interrupt.h"

// Equvalents of std::chrono::<resolution>, required because std::chrono uses 64bit integers and we don't have those. 
typedef std::chrono::duration<u32, std::ratio<1, 1000000000>> gba_nanoseconds;
typedef std::chrono::duration<u32, std::ratio<1, 1000000>> gba_microseconds;
typedef std::chrono::duration<u32, std::ratio<1, 1000>> gba_milliseconds;
typedef std::chrono::duration<u32, std::ratio<1, 1>> gba_seconds;

class GbaClock {
public:
    typedef std::chrono::duration<u32, std::ratio<1, 16780000>> duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<GbaClock> time_point;
    static const bool is_steady = true;

    static GbaClock& instance();

    std::chrono::time_point<GbaClock> now() noexcept;

    void handle_interrupt();

private:
    uint32_t rollovers;

    GbaClock();

    static void irq_handler();
};

#endif // GBA_CLOCK