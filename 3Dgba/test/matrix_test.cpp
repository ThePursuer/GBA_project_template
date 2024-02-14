#include <3Dgba/math/matrix.h>

#include <iostream>

#include "3Dgba/math/math.h"
#include "3Dgba/box3d.h"
#include "gba_os/QuickTimer.h"
#include "3Dgba/math/division_table_14_32.h"

#include "debug.h"

bool matrixSetBasis_asm_test(std::stringstream& err){
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

bool matrixLerp_test(std::stringstream& err){
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
   

    err << "New implementation took " << time << " nanoseconds" << std::endl;
    err << matrix_to_string(m) << std::endl;
    
    return false;
}