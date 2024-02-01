#ifndef MATH_3DGBA_H
#define MATH_3DGBA_H

#include <cstdint>

#define DIV_TABLE_SIZE   1025 // to compare with #1024 without extra LDR
extern uint16_t divTable[DIV_TABLE_SIZE];
extern const uint32_t gSinCosTable[4096];


#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t fix16_t;
fix16_t int_to_fix16(int32_t a);
int32_t fix16_to_int(fix16_t a);
fix16_t fix16_mul(fix16_t a, fix16_t b);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MATH_3DGBA_H
