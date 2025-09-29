/**
 * @file test_core_startup.c
 * @author jake-is-ESD-protected
 * @date 2024-03-04
 * @brief Tests for the initialization of jescore.
 *
 * @attention   This code tests jescore as a packaged system,
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


#include <unity.h>
#include "jescore.h"
#include "job_names.h"
#include "cli.h"
#include "base_jobs.h"
#include "core.h"


void test_core_init(void){
    jes_err_t stat = jes_init();
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    // Test double init
    stat = jes_init();
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
}


void test_job_core(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(CORE_JOB_NAME);
    TEST_ASSERT_EQUAL_STRING(CORE_JOB_NAME, pj->name);
    TEST_ASSERT_NOT_EQUAL(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(BOARD_MIN_JOB_HEAP_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__core_job, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(1, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(1, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_core, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_core, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_cli_init(void){
    jes_err_t e = __cli_init();
    TEST_ASSERT_EQUAL(e_err_no_err, e);
}


void test_job_help(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(HELP_NAME);
    TEST_ASSERT_EQUAL_STRING(HELP_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(BOARD_MIN_JOB_HEAP_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__base_job_help, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_base, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_job_stats(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(STATS_NAME);
    TEST_ASSERT_EQUAL_STRING(STATS_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(BOARD_MIN_JOB_HEAP_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__base_job_stats, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_base, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}

void test_sys_time(void){
    jes_delay_job_ms(200);
    volatile uint32_t sys_time = __get_systime_ms();
    TEST_ASSERT_GREATER_THAN_INT32(0, sys_time);
    TEST_ASSERT_INT32_WITHIN(4000, 500, sys_time);
}


#if __JES_LOG_LEN > 0
void test_logging(void){
    job_struct_t* pj = __job_get_job_by_name(STATS_NAME);
    __core_add_to_log_auto(pj, "test");
    log_entry_t le;
    le = __core_read_from_log_next();
    TEST_ASSERT_UINT32_WITHIN(500, 3000, le.sys_time);
    TEST_ASSERT_EQUAL_STRING("test", le.type);
    TEST_ASSERT_EQUAL_STRING(pj->name, le.job_state.name);
}
#endif