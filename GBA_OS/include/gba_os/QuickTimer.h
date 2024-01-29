#ifndef QUICKTIMER_H
#define QUICKTIMER_H

#include <cstdint>

#include <gba.h>

// Start the timer
inline void startTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value

    // Set control to start the timer with a 1:1 prescaler and enable interrupts if needed
    REG_TM0CNT_H = TIMER_START;
}

// Stop the timer and return elapsed time in clock cycles
inline u16 stopTimer() {
    // Disable the timer
    REG_TM0CNT_H &= ~TIMER_START;
    
    // Return the value of the timer
    return REG_TM0CNT_L;
}

// Reset the timer
inline void resetTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value
}

#endif // QUICKTIMER_H
