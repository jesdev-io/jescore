/**
 * @file test_runner.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-07
 * @brief Main test runner file.
 *
 * 
 */

#include <Arduino.h>
#include <unity.h>
#include "common_test.h"


void setUp(void) {

}


void tearDown(void) {

}


void setup()
{
    delay(2000);
    UNITY_BEGIN();
    delay(1000);

    // Backend startup
    RUN_TEST(test_core_init);
    delay(100);
    RUN_TEST(test_job_core);
    RUN_TEST(test_job_error_handler);
    RUN_TEST(test_job_init_cli);
    RUN_TEST(test_job_reprint_header);
    RUN_TEST(test_job_help);
    RUN_TEST(test_job_stats);

    // API functions
    RUN_TEST(test_register_job);
    RUN_TEST(test_launch_job);
    RUN_TEST(test_jesprint);
    RUN_TEST(test_set_get_args);
    RUN_TEST(test_set_get_params);
    
    UNITY_END();
}

void loop()
{
}