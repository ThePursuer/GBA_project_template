#include <gba.h>

#include <gba_os/UnitTest.h>
#include <gba_os/Console.h>
#include <gba_os/Core.h>

#include "test_registry.h"

EWRAM_CODE void run_console_test(){
    gba_os::init_gba_os();
    gba_os::console::initialize_console();

    // Register tests
    gba_os::test::RegisterTest("matrixSetBasis_test", &matrixSetBasis_asm_test);
    gba_os::test::RegisterTest("matrixRotateX_test", &matrixLerp_test);
    gba_os::test::RegisterTest("fix7_test", &fix7_test);
    gba_os::test::RegisterTest("fix14_test", &fix14_test);
    // gba_os::test::RegisterTest("sort_inplace_test", &sort_inplace_test);

    // Run all registered tests
    gba_os::test::RunTests();

    // Display the results
    gba_os::set_framerate(gba_os::video::FRAMERATE30);

    gba_os::Task test_results_task_;
    test_results_task_.f = &gba_os::test::test_results_task;
    test_results_task_.data = NULL;
    test_results_task_.priority = gba_os::TASK_PRIORITY::HIGH;
    gba_os::register_task(test_results_task_);

    gba_os::run_gba_os();
}


int main(){
    run_console_test();
    return 0;
}
