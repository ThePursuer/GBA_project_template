#include <3Dgba/math/fix7.h>
#include <3Dgba/math/fix14.h>

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
        err << "index: " << std::to_string(DIV_TABLE_7_16_INDEX(b)) << " shifted by: " << (7 + DIVISION_TABLE_7_16_PRECISION) << std::endl;
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

bool fix14_test(std::stringstream& err){
    fix14_t a = int_to_fix14(5);
    fix14_t b = int_to_fix14(4);

    if (a != 81920){
        err << "int_to_fix14 failed: " << a << " != " << int_to_fix14(5) << std::endl;
        return false;
    }

    if (fix14_to_int(a) != 5){
        err << "fix14_to_int failed: " << a << " != " << int_to_fix14(5) << std::endl;
        return false;
    }

    fix14_t c = fix14_mul(a, b);
    if(c != int_to_fix14(20)){
        err << "fix14_mul failed: " << a << "*" << b << "=" << c << std::endl;
        return false;
    }

    a = int_to_fix14(3);
    b = int_to_fix14(4);
    fix14_t d = fix14_div(a, b);
    if (d != 12288){
        err << "fix14_div failed: " << a << "/" << b << "=" << d << std::endl;
        err << "index: " << std::to_string(DIV_TABLE_14_32_INDEX(b)) << " shifted by: " << (14 + DIVISION_TABLE_14_32_PRECISION) << std::endl;
        return false;
    }

    if(fix14_sqrt(b) != 32768){
        err << "fix14_sqrt failed: " << b << "=" << fix14_sqrt(b) << std::endl;
        return false;
    }

    return true;
}
