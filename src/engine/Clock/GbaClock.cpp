#include "GbaClock.h"
#include <stdio.h>

GbaClock& GbaClock::instance() {
    static GbaClock clock;
    return clock;
}

std::chrono::time_point<GbaClock> GbaClock::now() noexcept {
    return time_point(duration((REG_TM0CNT_L + rollovers * 0x10000) * 1024));
}

void GbaClock::handle_interrupt() {
    rollovers++;
}

GbaClock::GbaClock() {
    rollovers = 0;

    REG_TM0CNT_L = 0; // reset timer value
    REG_TM0CNT_H = TIMER_START | TIMER_IRQ | TIMER_COUNT | 0b11; // start timer with interrupts enabled, maximum prescale
    irqSet(IRQ_TIMER0, &GbaClock::irq_handler);
    irqEnable(IRQ_TIMER0);
}

void GbaClock::irq_handler() {
    GbaClock::instance().handle_interrupt();
}