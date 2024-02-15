#ifndef QUICKTIMER_H
#define QUICKTIMER_H

#include <cstdint>

#include <gba.h>

// Start the timer
void startTimer();

// Stop the timer and return elapsed time in clock cycles
volatile u32 stopTimer();

// Reset the timer
void resetTimer();

#endif // QUICKTIMER_H
