#ifndef DIVISION_TABLE_7_16_H
#define DIVISION_TABLE_7_16_H

#include <cstdint>

#define DIVISION_TABLE_7_16_PRECISION 5
#define DIVISION_TABLE_7_16_STEP 2 // in powers of 2
#define DATA_TYPE_7_16_PRECISION_BITS 7
#define DIV_TABLE_7_16_INDEX(divisor) (((divisor) >> (DATA_TYPE_7_16_PRECISION_BITS - DIVISION_TABLE_7_16_STEP)) - 1)

extern int16_t division_table_7_16[1027];

#endif
