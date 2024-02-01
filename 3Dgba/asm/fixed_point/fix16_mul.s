#include "fix16_common_asm.inc"

.global fix16_mul
fix16_mul:
    // R0 = inArg0, R1 = inArg1
    // R2, R3 will be used for the product (R2: lower, R3: upper)
    SMULL R2, R3, R0, R1  // unsigned multiply with long result

    // Overflow check
    LDR R4, =#0x1FFF
    CMP R3, R4     // Check if upper 17 bits are the same (sign bit replicated)
    BGT end_overflow     // Branch to end if overflowed

    // Rounding
    TST R2, #0x2000      // Test 14th bit for rounding
    ADDEQ R2, R2, #1     // If set, add 1 for rounding

    // Shift and prepare final result

    MOV R3, R3, LSL #18   // Shift upper half to correct position
    MOV R2, R2, LSR #FIXED_SHIFT   // Shift lower half to correct position
    ORR R0, R3, R2        // Combine upper and lower half
    bx lr

end_overflow:
    LDR R0, =#fix16_overflow // Load overflow constant if not equal
    bx lr

.global fix16_to_int
fix16_to_int:
    // R0 = fix16_t value
    ASR R0, R0, #FIXED_SHIFT  // Arithmetic shift right by 16 bits to remove fractional part
    bx lr

.global int_to_fix16
int_to_fix16:
    // R0 = integer value
    LSL R0, R0, #FIXED_SHIFT  // Logical shift left by 16 bits to make space for fractional part
    bx lr
