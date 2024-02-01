.section .iwram
.arm

// .equ FOUR_DIV_PI, 0x145F3
// .equ _FOUR_DIV_PI2, 0xFFFF9840
// .equ X4_CORRECTION_COMPONENT, 0x399A
// .equ PI_DIV_4, 0x0000C90F
// .equ THREE_PI_DIV_4, 0x00025B2F
// 
// .equ fix16_maximum, 0x7FFFFFFF
// .equ fix16_minimum, 0x80000000
.equ fix16_overflow, 0x80000000
// 
// .equ fix16_pi, 205887
// .equ fix16_e, 178145
// .equ fix16_one, 0x00010000
// .equ fix16_eps, 1

.equ FIXED_SHIFT,   14

.macro fiq_on
    msr cpsr, #0x11     // switch r8-r14 to FIQ (IRQ enabled)
.endm

.macro fiq_off
    msr cpsr, #0x1F     // restore r8-r14
.endm

.macro fiq_on_ne
    msrne cpsr, #0x11   // switch r8-r14 to FIQ (IRQ enabled)
.endm

.macro fiq_off_ne
    msrne cpsr, #0x1F   // restore r8-r14
.endm
