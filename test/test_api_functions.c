/**
 * @file test_api_functions.c
 * @author jake-is-ESD-protected
 * @date 2024-03-07
 * @brief Test of all API functions located in `jescore_api.c`
 *
 * @attention The function `jes_init()` is already tested in `test_core_startup.c`
 */


#include <unity.h>
#include "jescore_api.h"
#include "job_names.h"
#include "base_jobs.h"

#define DUMMY_JOB_SINGLE_NAME "dummysingle"
#define DUMMY_JOB_LOOP_NAME "dummyloop"
#define DUMMY_JOB_ARGS_HOLDER_NAME "dummyargs"
#define DUMMY_JOB_NOTIFY "dummynotify"
#define DUMMY_JOB_NOTIFY_TAKE "dummynotifytake"
#define DUMMY_NOTIFICATION_VALUE 1234
#define DUMMY_PRINT "hello world!"
#define DUMMY_FAIL_MSG "Assert failed"
#define DUMMY_SUCCESS_MSG "Assert succeeded"
#define DUMMY_ARGS "args at launch"
#define DUMMY_ARGS_MODIF "got args at launch"
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


void dummy_job_args_holder(void* p){
    char* args = get_args();
    if(strcmp(DUMMY_ARGS, args) == 0){
        set_args((char*)DUMMY_ARGS_MODIF);
    }
    for(uint8_t i = 0; i < 255; i++){
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}


void dummy_job_notify(void* p){
    uint32_t val = DUMMY_NOTIFICATION_VALUE;
    notify_job(DUMMY_JOB_NOTIFY_TAKE, &val);
}


void dummy_job_notify_take(void* p){
    uint32_t* pval;
    pval = (uint32_t*)wait_for_notification();
    set_param(pval);
    uint32_t* pval_equal = (uint32_t*)job_get_param();
    if(*pval != *pval_equal){
        set_args((char*)DUMMY_FAIL_MSG);
    }
    else{
        set_args((char*)DUMMY_SUCCESS_MSG);
    }
}


void test_register_job(void){
    jes_err_t stat = register_job(DUMMY_JOB_SINGLE_NAME,
                                  DUMMY_JOB_MEM,
                                  DUMMY_JOB_PRIO,
                                  dummy_job_single,
                                  0);
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
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = register_job(DUMMY_JOB_LOOP_NAME,
                        DUMMY_JOB_MEM,
                        DUMMY_JOB_PRIO,
                        dummy_job_loop,
                        1);
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
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = register_job(DUMMY_JOB_ARGS_HOLDER_NAME,
                        DUMMY_JOB_MEM,
                        DUMMY_JOB_PRIO,
                        dummy_job_args_holder,
                        0);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);

    pj = __job_get_job_by_name(DUMMY_JOB_ARGS_HOLDER_NAME);
    TEST_ASSERT_EQUAL_STRING(DUMMY_JOB_ARGS_HOLDER_NAME, pj->name);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->handle);
    TEST_ASSERT_EQUAL_UINT32(DUMMY_JOB_MEM, pj->mem_size);
    TEST_ASSERT_EQUAL_UINT8(DUMMY_JOB_PRIO, pj->priority);
    TEST_ASSERT_EQUAL_HEX32(dummy_job_args_holder, pj->function);    
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);
    TEST_ASSERT_EQUAL_UINT8(0, pj->is_loop);
    TEST_ASSERT_EQUAL_UINT8(0, pj->instances);
    TEST_ASSERT_EQUAL_INT(e_role_user, pj->role);
    TEST_ASSERT_EQUAL_INT(e_origin_undefined, pj->caller);
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
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
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);

    stat = launch_job(DUMMY_JOB_LOOP_NAME);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    vTaskDelay(200 / portTICK_PERIOD_MS);   // let job start

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
    TEST_ASSERT_EQUAL_HEX32(NULL, pj->param);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, pj->error);
}


void test_set_get_args(void){
    char dummy[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    strcpy(dummy, DUMMY_PRINT);
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);

    jes_err_t stat = __job_set_args((char*)DUMMY_PRINT, pj);    // API call only works within jobs
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, pj->args, __MAX_JOB_ARGS_LEN_BYTE);

    char* args = __job_get_args(pj);    // API call only works within jobs
    TEST_ASSERT_EQUAL_INT8_ARRAY(dummy, args, __MAX_JOB_ARGS_LEN_BYTE);
}


void test_set_job_get_params(void){
    uint32_t value = 1000;
    job_struct_t* pj = __job_get_job_by_name(DUMMY_JOB_SINGLE_NAME);

    jes_err_t stat = __job_set_param(&value, pj);   // API call only works within jobs
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    uint32_t* internal_val = (uint32_t*)pj->param;
    TEST_ASSERT_EQUAL_INT(value, *internal_val);

    uint32_t ret = *(uint32_t*)__job_get_param(pj); // API call only works within jobs
    TEST_ASSERT_EQUAL_INT(value, ret);
}


void test_launch_job_args(void){
    jes_err_t stat = launch_job_args(DUMMY_JOB_ARGS_HOLDER_NAME,
                                     DUMMY_ARGS);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    char* args = __job_get_args(__job_get_job_by_name(DUMMY_JOB_ARGS_HOLDER_NAME));
    TEST_ASSERT_EQUAL_STRING(DUMMY_ARGS_MODIF, args);
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


void test_notify_job_and_wait(void){
    jes_err_t stat = register_and_launch_job(DUMMY_JOB_NOTIFY_TAKE,
                                             DUMMY_JOB_MEM,
                                             DUMMY_JOB_PRIO,
                                             dummy_job_notify_take,
                                             0);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    stat = register_and_launch_job(DUMMY_JOB_NOTIFY,
                                   DUMMY_JOB_MEM,
                                   DUMMY_JOB_PRIO,
                                   dummy_job_notify,
                                   0);
    TEST_ASSERT_EQUAL_INT(e_err_no_err, stat);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    uint32_t* pval;
    pval = (uint32_t*)__job_get_param(__job_get_job_by_name(DUMMY_JOB_NOTIFY_TAKE));
    char* msg = __job_get_args(__job_get_job_by_name(DUMMY_JOB_NOTIFY_TAKE));
    TEST_ASSERT_EQUAL_INT32(DUMMY_NOTIFICATION_VALUE, *pval);
    TEST_ASSERT_EQUAL_STRING(DUMMY_SUCCESS_MSG, msg);
}