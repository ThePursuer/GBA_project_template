#include "gba_os/QuickTimer.h"

// Start the timer
void startTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value

    // Set control to start the timer with a 1:1 prescaler and enable interrupts if needed
    REG_TM0CNT_H = TIMER_START | 0b00;
}

// Stop the timer and return elapsed time in clock cycles
volatile u32 stopTimer() {
    // Disable the timer
    REG_TM0CNT_H &= ~TIMER_START;

    // Return the value of the timer
    return (REG_TM0CNT_L - 107) * 59.595 ; // time it takes to start/stop the timer
}

// Reset the timer
void resetTimer() {
    REG_TM0CNT_H = 0; // Disable timer
    REG_TM0CNT_L = 0; // Reset timer value
}
