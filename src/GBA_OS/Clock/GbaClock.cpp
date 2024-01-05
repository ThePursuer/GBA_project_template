#include "GbaClock.h"

u32 GbaClock::rollovers = 0;

GbaClock& GbaClock::instance() {
    static GbaClock clock;
    return clock;
}

std::chrono::time_point<GbaClock> GbaClock::now() noexcept {
    return time_point(duration(((REG_TM3CNT_L + rollovers * 0x10000) * 10) / 164));
}

void GbaClock::handle_interrupt() {
    rollovers++;
}

GbaClock::GbaClock() {
    REG_TM3CNT_L = 0; // reset timer value
    REG_TM3CNT_H = TIMER_START | TIMER_IRQ | TIMER_COUNT | 0b11; // start timer with interrupts enabled, maximum prescale
    REG_TM3CNT_H &= ~(1 << 2);
    irqSet(IRQ_TIMER3, &GbaClock::irq_handler);
    irqEnable(IRQ_TIMER3);
}

void GbaClock::irq_handler() {
    GbaClock::instance().handle_interrupt();
}