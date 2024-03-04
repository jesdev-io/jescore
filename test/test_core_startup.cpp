/**
 * @file test_core_startup.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-04
 * @brief Tests for the initialization of jes-core.
 *
 * @attention   This code tests jes-core as a packaged system,
 *              meaning that it is present in the background of
 *              all tests as a static variable until all tests
 *              in this file are finished. Since the core
 *              variable is designed to act purely in the
 *              background, no handles exist to probe it directly.
 *              Instead, its parameters and jobs are tested with
 *              the provided API. 
 * 
 *              Since the core's state is technically probed
 *              AFTER init, some tests may look a bit confusing,
 *              as they assert the existence of a few jobs but
 *              without any instances, even though they occurred.
 */


#include <Arduino.h>
#include <unity.h>
#include "jescore.h"
#include "job_names.h"
#include "cli.h"
#include "base_jobs.h"


void setUp(void) {

}


void tearDown(void) {

}


void test_core_init(void){
    jes_err_t stat = jes_init();
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
}


void test_job_core(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(CORE_JOB_NAME);
    TEST_ASSERT_EQUAL_STRING(CORE_JOB_NAME, pj->name);
    TEST_ASSERT_NOT_EQUAL(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(2048, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__core_job, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(1, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(1, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_core, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_error_handler(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(ERROR_HANDLER_NAME);
    TEST_ASSERT_EQUAL_STRING(ERROR_HANDLER_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(1024, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__core_job_err_handler, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_core, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_init_cli(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(INIT_CLI_JOB_NAME);
    TEST_ASSERT_EQUAL_STRING(INIT_CLI_JOB_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(2048, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(init_cli, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_core, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_reprint_header(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(HEADER_PRINTER_NAME);
    TEST_ASSERT_EQUAL_STRING(HEADER_PRINTER_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(1024, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(reprint_header, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_core, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_help(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(HELP_NAME);
    TEST_ASSERT_EQUAL_STRING(HELP_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(2048, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__base_job_help, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_base, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_stats(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(STATS_NAME);
    TEST_ASSERT_EQUAL_STRING(STATS_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(4096, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__base_job_stats, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_base, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void setup()
{
    delay(2000);
    UNITY_BEGIN();
    delay(1000);

    RUN_TEST(test_core_init);
    delay(100);
    RUN_TEST(test_job_core);
    RUN_TEST(test_job_error_handler);
    RUN_TEST(test_job_init_cli);
    RUN_TEST(test_job_reprint_header);
    RUN_TEST(test_job_help);
    RUN_TEST(test_job_stats);
    
    UNITY_END();
}

void loop()
{
}