#ifndef DIVISION_TABLE_H
#define DIVISION_TABLE_H

#include <cstdint>

#define DIVISION_TABLE_PRECISION 5
#define DIVISION_TABLE_STEP 2 // in powers of 2
#define DIVISION_TABLE_PRECISION_BITS 7
#define DIV_TABLE_INDEX(divisor) (((divisor) >> (DIVISION_TABLE_PRECISION_BITS - DIVISION_TABLE_STEP)) - 1)

extern int16_t division_table[1027];

#endif
