/**
 * @file common_test.c
 * @author jake-is-ESD-protected
 * @date 2025-03-06
 * @brief Common main test routine for all platforms/frameworks.
 *
 * 
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <unity.h>
#include "common_test.h"


void setUp(void) {

}


void tearDown(void) {

}


void test_all(void) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    UNITY_BEGIN();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Backend startup
    RUN_TEST(test_core_init);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    RUN_TEST(test_job_core);
    RUN_TEST(test_job_error_handler);
    RUN_TEST(test_job_init_cli);
    RUN_TEST(test_job_reprint_header);
    RUN_TEST(test_job_help);
    RUN_TEST(test_job_stats);

    // API functions
    RUN_TEST(test_register_job);
    RUN_TEST(test_launch_job);
    RUN_TEST(test_set_get_args);
    RUN_TEST(test_set_job_get_params);
    RUN_TEST(test_launch_job_args);
    RUN_TEST(test_core_job_launch_prohibited);
    RUN_TEST(test_notify_job_and_wait);
    
    UNITY_END();
}