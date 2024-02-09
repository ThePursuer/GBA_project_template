#include <3Dgba/matrix.h>

#include <iostream>

#include "3Dgba/math.h"
#include "3Dgba/box3d.h"

#include "debug.h"

bool matrixSetBasis_asm_test(std::ostringstream& err){
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

bool matrixRotateX_asm_test(std::ostringstream& err){
    
    return true;
}