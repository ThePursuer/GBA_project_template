#include "gba_os/Clock.h"
#include "interrupt_manager.h"

namespace Gba_os::Clock {

static volatile uint64_t rollovers = 0;

GbaClock& GbaClock::instance() {
    static IWRAM_DATA GbaClock clock;
    return clock;
}

IWRAM_CODE ARM_CODE std::chrono::time_point<GbaClock> GbaClock::now() noexcept {
    // 59.595 nanoseconds per timer tick
    // muliplying by 59595 / 1000 lets us avoid floating point math
    // the ratio: 59595 / 1000 == 61025 / 1024 gives us a power of 2 for right shift division
    // therefor (tick * 61025) >> 10 == (tick * 59.595), no fp math and no division
    return time_point(duration((((REG_TM3CNT_L + rollovers) * 61025) >> 10))); 
}

GbaClock::GbaClock() {
    InterruptManager::instance();

    REG_TM3CNT_H = 0;
    REG_TM3CNT_L = 0; // reset timer value

    irqEnable(IRQ_TIMER3);
    irqSet(IRQ_TIMER3, &GbaClock::irq_handler);
    REG_TM3CNT_H = TIMER_START | TIMER_IRQ | 0b00; // start timer with interrupts enabled, maximum prescale
}

IWRAM_CODE void GbaClock::irq_handler() {
    rollovers += 0x10000;
}

} // gba_os::clock
