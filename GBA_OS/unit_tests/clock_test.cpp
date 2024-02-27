#include <gba_os/Clock.h>
#include <gba_os/SimpleOutputStream.h>

using namespace Gba_os::Clock;
using namespace Gba_os::console;
static GbaClock& my_clock = GbaClock::instance();

bool clock_test(SimpleOutputStream& err){
    auto start = my_clock.now();
    for (volatile int i = 0; i < 10000; i++){}
    auto end = my_clock.now();

    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    if (diff < 1000){
        err << "Clock diff is wrong: " << diff << std::endl;
        err << "start: " << start.time_since_epoch().count() << " end: " << end.time_since_epoch().count() << std::endl;
        return false;
    }

    start = my_clock.now();
    end = my_clock.now();
    diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if (diff > 9){
        err << "Clock call time is unexpectedly high" << diff << std::endl;
        return false;
    }

    return true;
}

bool gba_units_compare_test(SimpleOutputStream& err){
    std::chrono::milliseconds ms(1);
    std::chrono::microseconds us(1000);

    if (ms != us){
        err << "Milliseconds and microseconds are not equal: " << ms.count() << " " << us.count() << std::endl;
        return false;
    }
}
