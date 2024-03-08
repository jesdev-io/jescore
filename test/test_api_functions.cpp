/**
 * @file test_api_functions.cpp
 * @author jake-is-ESD-protected
 * @date 2024-03-07
 * @brief Test of all API functions located in `jescore_api.cpp`
 *
 * @attention The function `jes_init()` is already tested in `test_core_startup.cpp`
 */


#include <Arduino.h>
#include <unity.h>
#include "jescore_api.h"
#include "job_names.h"
#include "base_jobs.h"

#define DUMMY_JOB_SINGLE_NAME "dummysingle"
#define DUMMY_JOB_LOOP_NAME "dummyloop"
#define DUMMY_PRINT "hello world!"
#define DUMMY_JOB_MEM 2048
#define DUMMY_JOB_PRIO 1


void dummy_job_single(void* p){
    for(uint8_t i = 0; i < 255; i++){
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}


void dummy_job_loop(void* p){
    while(1){
        for(uint8_t i = 0; i < 255; i++){
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
    }
}


void test_register_job(void){
    jes_err_t stat = register_job(DUMMY_JOB_SINGLE_NAME,
                                  DUMMY_JOB_MEM,
                                  DUMMY_JOB_PRIO,
                                  dummy_job_single,
                                  false);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);

    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);
    TEST_ASSERT_EQUAL_STRING(DUMMY_JOB_SINGLE_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(DUMMY_JOB_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(DUMMY_JOB_PRIO, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(dummy_job_single, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_user, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = register_job(DUMMY_JOB_LOOP_NAME,
                                  DUMMY_JOB_MEM,
                                  DUMMY_JOB_PRIO,
                                  dummy_job_loop,
                                  true);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);

    pj = __job_get_job_by_name(DUMMY_JOB_LOOP_NAME);
    TEST_ASSERT_EQUAL_STRING(DUMMY_JOB_LOOP_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(DUMMY_JOB_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(DUMMY_JOB_PRIO, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(dummy_job_loop, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(1, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_user, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_launch_job(void){
    jes_err_t stat = launch_job(DUMMY_JOB_SINGLE_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);

    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);
    TEST_ASSERT_EQUAL_STRING(DUMMY_JOB_SINGLE_NAME, pj->name);
    TEST_ASSERT_NOT_EQUAL(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(DUMMY_JOB_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(DUMMY_JOB_PRIO, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(dummy_job_single, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(1, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_user, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_api, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = launch_job(DUMMY_JOB_LOOP_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);

    pj = __job_get_job_by_name(DUMMY_JOB_LOOP_NAME);
    TEST_ASSERT_EQUAL_STRING(DUMMY_JOB_LOOP_NAME, pj->name);
    TEST_ASSERT_NOT_EQUAL(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(DUMMY_JOB_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(DUMMY_JOB_PRIO, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(dummy_job_loop, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(1, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(1, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_user, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_api, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_jesprint(void){
    jes_err_t stat = jesprint(DUMMY_PRINT);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    strcpy(dummy, DUMMY_PRINT);
    job_struct_t* pj = __job_get_job_by_name(PRINT_JOB_NAME);
    TEST_ASSERT_EQUAL_STRING(PRINT_JOB_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(4096, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(1, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(__base_job_echo, pj->function);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_base, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_api, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->optional);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = jesprint("This string is too long and should therefore not be printable in the current state of jesprint!");
    TEST_ASSERT_EQUAL_INT(e_err_too_long, stat);
}


void test_set_get_args(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    strcpy(dummy, DUMMY_PRINT);
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);

    jes_err_t stat = set_args((char*)DUMMY_PRINT, pj);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);

    char* args = get_args(pj);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, args, __MAX_JOB_ARGS_LEN_BYTE);
}


void test_set_get_params(void){
    uint32_t value = 1000;
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);

    jes_err_t stat = set_param(&value, pj);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    uint32_t* internal_val = (uint32_t*)pj->optional;
    TEST_ASSERT_EQUAL_INT(value, *internal_val);

    uint32_t ret = *(uint32_t*)get_param(pj);
    TEST_ASSERT_EQUAL_INT(value, ret);
}


void test_core_job_launch_prohibited(void){
    jes_err_t stat = launch_job(CORE_JOB_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_prohibited, stat);
    stat = launch_job(ERROR_HANDLER_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_prohibited, stat);
    stat = launch_job(INIT_CLI_JOB_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_prohibited, stat);
    stat = launch_job(SERIAL_READ_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_prohibited, stat);
    stat = launch_job(HEADER_PRINTER_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_prohibited, stat);
}