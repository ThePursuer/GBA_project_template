#include <3Dgba/math.h>

#include <iostream>
#include <sstream>
#include <string>

#include <gba_os/QuickTimer.h>

bool fix7_test(std::stringstream& err){
    fix7_t a = int_to_fix7(5);
    fix7_t b = int_to_fix7(4);

    if (a != 640){
        err << "int_to_fix7 failed: " << a << " != " << int_to_fix7(5) << std::endl;
        return false;
    }

    if (fix7_to_int(a) != 5){
        err << "fix7_to_int failed: " << a << " != " << int_to_fix7(5) << std::endl;
        return false;
    }

    fix7_t c = fix7_mul(a, b);
    if(c != 2560){
        err << "fix7_mul failed: " << a << "*" << b << "=" << c << std::endl;
        return false;
    }

    a = int_to_fix7(3);
    b = int_to_fix7(4);
    fix7_t d = fix7_div(a, b);
    if (d != 96){
        err << "fix7_div failed: " << a << "/" << b << "=" << d << std::endl;
        err << "index: " << std::to_string((b >> 5) - 1) << " shifted by: " << (7 + DIVISION_TABLE_PRECISION) << std::endl;
        return false;
    }

    if(fix7_sqrt(b) != 256){
        err << "fix7_sqrt failed: " << b << "=" << fix7_sqrt(b) << std::endl;
        return false;
    }

    if(fix7_to_int(b) != 4){
        err << "fix7_to_int failed: " << b << " != " << int_to_fix7(4) << std::endl;
        return false;
    }

    return true;
}

__always_inline void fix7_mul_c(fix7_t &a, fix7_t b){
    a = ((a >> 3) * (b >> 3));
};
__always_inline fix7_t fix7_mul_asm(fix7_t a, const fix7_t b){
        asm volatile(
        "asr %[input_a], %[input_a], #3\n"
        "asr %[input_b], %[input_b], #3\n"
        "mul %[result], %[input_a], %[input_b]\n"
        : [result] "+r" (a)
        : [input_a] "r" (a), [input_b] "r" (b)
        : "cc" // Condition code flags are affected by the add operation
    );
    return a;
};

bool ARM_CODE assem_test(std::stringstream& err){
    volatile fix14_t a = int_to_fix14(100);
    volatile fix14_t b = int_to_fix14(3);
    volatile fix14_t c;

    volatile u32 time;

    startTimer();
    c = fix14_mul(a, b);
    time = stopTimer();

    err << "slow?: " << std::to_string(fix14_to_int(c))  << " raw: " << std::to_string(c) << " time: " << std::to_string(time) << std::endl;

    startTimer();
    c = fix14_mul_fast(a, b);
    time = stopTimer();

    err << "fast: " << std::to_string(fix14_to_int(c))  << " raw: " << std::to_string(c) << " time: " << std::to_string(time) << std::endl;

    return false;
}