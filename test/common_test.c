/**
 * @file common_test.c
 * @author jake-is-ESD-protected
 * @date 2025-03-06
 * @brief Common main test routine for all platforms/frameworks.
 *
 * 
 */

#include "board_parser.h"
#ifdef BUILD_FOR_STM32
#include "task.h"
#include "queue.h"
#endif
#include <unity.h>
#include "common_test.h"
#include "delay_unif.h"


void setUp(void) {

}


void tearDown(void) {

}


void test_all(void* p) {
    jes_delay_job_ms(2000);
    UNITY_BEGIN();
    jes_delay_job_ms(1000);

    // Backend startup
    RUN_TEST(test_core_init);
    jes_delay_job_ms(100);
    RUN_TEST(test_job_core);
    RUN_TEST(test_job_error_handler);
    RUN_TEST(test_cli_init);
    RUN_TEST(test_job_help);
    RUN_TEST(test_job_stats);

    // API functions
    RUN_TEST(test_register_job);
    RUN_TEST(test_launch_job);
    RUN_TEST(test_set_get_args);
    RUN_TEST(test_set_job_get_params);
    RUN_TEST(test_launch_job_args);
    RUN_TEST(test_error_throw_get);
    RUN_TEST(test_core_job_launch_prohibited);
    RUN_TEST(test_notify_job_and_wait);
    
    UNITY_END();
}