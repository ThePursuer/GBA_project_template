#ifndef DIVISION_TABLE_14_32_H
#define DIVISION_TABLE_14_32_H

#include <cstdint>

#define DIVISION_TABLE_14_32_PRECISION 10
#define DIVISION_TABLE_14_32_STEP 2 // in powers of 2
#define DATA_TYPE_14_32_PRECISION_BITS 14
#define DIV_TABLE_14_32_INDEX(divisor) (((divisor) >> (DATA_TYPE_14_32_PRECISION_BITS - DIVISION_TABLE_14_32_STEP)) - 1)

extern int32_t division_table_14_32[1027];

#endif
