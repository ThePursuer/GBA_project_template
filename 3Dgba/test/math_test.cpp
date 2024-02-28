#include <3Dgba/math/fix7.h>
#include <3Dgba/math/fix14.h>
#include <3Dgba/data_types.h>

#include <iostream>
#include <sstream>
#include <string>

#include <gba_os/QuickTimer.h>
#include <gba_os/SimpleOutputStream.h>

using namespace Gba_os::console;

bool fix7_test(SimpleOutputStream& err){
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

bool fix14_test(SimpleOutputStream& err){
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

bool Vector2_test(SimpleOutputStream& err){
    Vector2<fix14_t> a = { int_to_fix14(5), int_to_fix14(4) };
    Vector2<fix14_t> b = { int_to_fix14(3), int_to_fix14(4) };

    Vector2<fix14_t> c = a + b;

    if (c.x != int_to_fix14(8) || c.y != int_to_fix14(8)){
        err << "Vector2 addition failed: " << c.x << ", " << c.y << " != " << int_to_fix14(8) << ", " << int_to_fix14(8) << std::endl;
        return false;
    }

    c = a - b;

    if (c.x != int_to_fix14(2) || c.y != int_to_fix14(0)){
        err << "Vector2 subtraction failed: " << c.x << ", " << c.y << " != " << int_to_fix14(2) << ", " << int_to_fix14(0) << std::endl;
        return false;
    }

    if(a == b){
        err << "Vector2 equality failed: " << a.x << ", " << a.y << " == " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if(!(a == a)){
        err << "Vector2 equality failed: " << a.x << ", " << a.y << " != " << a.x << ", " << a.y << std::endl;
        return false;
    }

    if(a != a){
        err << "Vector2 inequality failed: " << a.x << ", " << a.y << " != " << a.x << ", " << a.y << std::endl;
        return false;
    }

    if(!(a != b)){
        err << "Vector2 inequality failed: " << a.x << ", " << a.y << " == " << b.x << ", " << b.y << std::endl;
        return false;
    }

    // Test comparison on same y axis
    if (a < b){
        err << "Vector2 less than failed: " << a.x << ", " << a.y << " < " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if(a < a){
        err << "Vector2 less than failed: " << a.x << ", " << a.y << " < " << a.x << ", " << a.y << std::endl;
        return false;
    }

    if (!(a > b)){
        err << "Vector2 greater than failed: " << a.x << ", " << a.y << " > " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if(a > a){
        err << "Vector2 greater than failed: " << b.x << ", " << b.y << " < " << a.x << ", " << a.y << std::endl;
        return false;
    }

    if (a <= b){
        err << "Vector2 less than or equal failed: " << a.x << ", " << a.y << " <= " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if(!(a <= a)){
        err << "Vector2 less than or equal failed: " << a.x << ", " << a.y << " <= " << a.x << ", " << a.y << std::endl;
        return false;
    }

    if (!(a >= b)){
        err << "Vector2 greater than or equal failed: " << a.x << ", " << a.y << " >= " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if(!(a >= a)){
        err << "Vector2 greater than or equal failed: " << b.x << ", " << b.y << " >= " << a.x << ", " << a.y << std::endl;
        return false;
    }

    // test comparisons on different y axis
    
    a = { int_to_fix14(5), int_to_fix14(4) };
    b = { int_to_fix14(3), int_to_fix14(6) };

    if (!(a < b)){
        err << "Vector2 less than failed: " << a.x << ", " << a.y << " < " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if (a > b){
        err << "Vector2 greater than failed: " << a.x << ", " << a.y << " > " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if (!(a <= b)){
        err << "Vector2 less than or equal failed: " << a.x << ", " << a.y << " <= " << b.x << ", " << b.y << std::endl;
        return false;
    }

    if (a >= b){
        err << "Vector2 greater than or equal failed: " << a.x << ", " << a.y << " >= " << b.x << ", " << b.y << std::endl;
        return false;
    }
}
