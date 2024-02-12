#include "assem_test.h"

#include <gba.h>

fix7_t a; 
fix7_t b;

ARM_CODE void user_func(){
    fix7_mul(a, b);
}

ARM_CODE void reg_func_imple() {
    fix7_mul_asm(a, b);
}
