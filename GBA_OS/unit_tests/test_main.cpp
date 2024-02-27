#include <gba.h>

#include <gba_os/UnitTest.h>
#include <gba_os/Console.h>
#include <gba_os/Core.h>

#include "test_registry.h"

void run_console_test(){
    Gba_os::init_gba_os();
    Gba_os::console::initialize_console();

    // Register tests
    Gba_os::test::RegisterTest("clock_units_compare", &gba_units_compare_test);
    Gba_os::test::RegisterTest("clock_test", &clock_test);

    // Run all registered tests
    Gba_os::test::RunTests();

    // // Display the results
    Gba_os::set_framerate(Gba_os::video::FRAMERATE15);

    Gba_os::Task test_results_task_;
    test_results_task_.f = &Gba_os::test::test_results_task;
    test_results_task_.data = NULL;
    test_results_task_.priority = Gba_os::TASK_PRIORITY::HIGH;
    Gba_os::register_task(test_results_task_);

    Gba_os::waitNextFrame();
    while (1)
    {
        // Gba_os::test::test_results_task(test_results_task_);
        Gba_os::tick();
    }
}


int main(){
    run_console_test();
    return 0;
}