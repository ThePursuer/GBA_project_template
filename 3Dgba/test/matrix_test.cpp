#include <iostream>

#include <gba_os/QuickTimer.h>
#include <gba_os/SimpleOutputStream.h>

#include "3Dgba/math/math.h"
#include "3Dgba/math/division_table_14_32.h"
#include "3Dgba/math/matrix.h"

#include "debug.h"

using namespace Gba_os::console;

bool matrixSetBasis_test(SimpleOutputStream& err){
    Matrix m{}, n{};
    m.e00 = 1;
    m.e01 = 2;
    m.e02 = 3;
    m.e10 = 4;
    m.e11 = 5;
    m.e12 = 6;
    m.e20 = 7;
    m.e21 = 8;
    m.e22 = 9;

    m.e03 = 0;
    m.e13 = 0;
    m.e23 = 0;

    matrixSetBasis_c(n, m);
    if (n.e00 == m.e00 && n.e01 == m.e01 && n.e02 == m.e02 &&
     n.e10 == m.e10 && n.e11 == m.e11 && n.e12 == m.e12 &&
      n.e20 == m.e20 && n.e21 == m.e21 && n.e22 == m.e22)
        return true;

    err << "got:" << std::endl;
    err << matrix_to_string_raw(n) << std::endl;
    err << "expected:" << std::endl;
    err << matrix_to_string_raw(m);
    return false;
}

bool matrixLerp_test(SimpleOutputStream& err){
    // todo: finish this
    Matrix m{}, n{};
    m.e00 = int_to_fix14(1); m.e01 = int_to_fix14(2); m.e02 = int_to_fix14(3); m.e03 = int_to_fix14(0);
    m.e10 = int_to_fix14(4); m.e11 = int_to_fix14(5); m.e12 = int_to_fix14(6); m.e13 = int_to_fix14(0);
    m.e20 = int_to_fix14(7); m.e21 = int_to_fix14(8); m.e22 = int_to_fix14(9); m.e23 = int_to_fix14(0);

    // Initialize the second matrix for linear interpolation calculation
    n.e00 = int_to_fix14(10); n.e01 = int_to_fix14(11); n.e02 = int_to_fix14(12); n.e03 = int_to_fix14(0);
    n.e10 = int_to_fix14(13); n.e11 = int_to_fix14(14); n.e12 = int_to_fix14(15); n.e13 = int_to_fix14(0);
    n.e20 = int_to_fix14(16); n.e21 = int_to_fix14(17); n.e22 = int_to_fix14(18); n.e23 = int_to_fix14(0);

    uint32_t time = 0;
    
    startTimer();
    matrixLinearInterpolation(m, n, 5461);
    time = stopTimer();
   
    bool pass = fix14_to_int(m.e00) == 3 && fix14_to_int(m.e01) == 4 && fix14_to_int(m.e02) == 5 &&
                fix14_to_int(m.e10) == 6 && fix14_to_int(m.e11) == 7 && fix14_to_int(m.e12) == 8 &&
                fix14_to_int(m.e20) == 9 && fix14_to_int(m.e21) == 10 && fix14_to_int(m.e22) == 11;
    
    if(!pass){
        err << "got:" << std::endl;
        err << matrix_to_string_raw(m) << std::endl;
        err << "expected:" << std::endl;
        err << "3 4 5" << std::endl;
        err << "6 7 8" << std::endl;
        err << "9 10 11" << std::endl;
        return false;
    }
    
    return true;
}