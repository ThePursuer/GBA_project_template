#include <gba.h>

#include <gba_os/UnitTest.h>
#include <gba_os/Console.h>
#include <gba_os/Core.h>

#include "test_registry.h"

EWRAM_CODE void run_console_test(){
    Gba_os::init_gba_os();
    Gba_os::console::initialize_console();

    // Register tests
    Gba_os::test::RegisterTest("matrixSetBasis_test", &matrixSetBasis_test);
    Gba_os::test::RegisterTest("matrixRotateX_test", &matrixLerp_test);
    Gba_os::test::RegisterTest("fix7_test", &fix7_test);
    Gba_os::test::RegisterTest("fix14_test", &fix14_test);
    Gba_os::test::RegisterTest("Vector2_test", &Vector2_test);
    Gba_os::test::RegisterTest("sort_inplace_test", &sort_inplace_test);

    // Run all registered tests
    auto results = Gba_os::test::RunTests();

    // Display the results
    Gba_os::set_framerate(Gba_os::video::FRAMERATE30);

    Gba_os::Task test_results_task_;
    test_results_task_.f = &Gba_os::test::test_results_task;
    test_results_task_.data = &results;
    test_results_task_.priority = Gba_os::TASK_PRIORITY::HIGH;
    Gba_os::register_task(test_results_task_);

    Gba_os::waitNextFrame();
    while(1){
        Gba_os::tick();
    }
}

int main(){
    run_console_test();
    return 0;
}
