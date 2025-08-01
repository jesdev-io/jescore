#include "jescore_api.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"
#include "base_jobs.h"


jes_err_t jes_init(void){
    return __core_init();
}

#ifdef BUILD_FOR_STM32

void jes_dispatch(void){
    vTaskStartScheduler();
}

#endif // BUILD_FOR_STM32


jes_err_t jes_register_job(const char* name,
                       uint32_t mem_size,
                       uint8_t priority,
                       void (*function)(void* p),
                       uint8_t is_loop){
    return __job_register_job(name,
                              mem_size,
                              priority,
                              function,
                              is_loop,
                              e_role_user);
    }


jes_err_t jes_launch_job(const char* name){
    job_struct_t* pj = __job_get_job_by_name(name);
    if(pj == NULL) { return e_err_unknown_job; }
    pj->caller = e_origin_api;
    __core_notify(pj, 0);
    return e_err_no_err;
}


jes_err_t jes_launch_job_args(const char* name, const char* args){
    if (args == NULL) { return e_err_is_zero; }
    job_struct_t* pj = __job_get_job_by_name(name);
    if(pj == NULL) { return e_err_unknown_job; }
    jes_err_t e = __job_set_args((char*)args, pj);
    if (e != e_err_no_err) { return e; }
    pj->caller = e_origin_api;
    __core_notify(pj, 0);
    return e_err_no_err;
}


jes_err_t jes_register_and_launch_job(const char* name,
                                  uint32_t mem_size,
                                  uint8_t priority,
                                  void (*function)(void* p),
                                  uint8_t is_loop){
    jes_err_t stat = __job_register_job(name,
                                        mem_size,
                                        priority,
                                        function,
                                        is_loop,
                                        e_role_user);
    if(stat != e_err_no_err){ return stat; }
    return jes_launch_job(name);
}


jes_err_t jes_job_set_args(char* s){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_args(s, pj);
}


char* jes_job_get_args(void){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_args(pj);
}


char* jes_job_arg_next(void) {
    static char* input = NULL;
    char* token;
    if (input == NULL) {
        input = jes_job_get_args();
        if (input == NULL) return NULL;
        token = strtok(input, " ");
    } else {
        token = strtok(NULL, " ");
    }
    if (token == NULL) {
        input = NULL;
    }
    return token;
}


uint8_t jes_job_is_arg(char* arg, const char* name){
    return strcmp((const char*)arg, name) == 0;
}


jes_err_t jes_job_set_param(void* p){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_param(p, pj);
}


void* jes_job_get_param(void){
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_param(pj);
}


jes_err_t jes_error_get(char* job_name){
    return __core_error_get(job_name);
}


jes_err_t jes_error_get_any(void){
    return __core_error_get_any();
}


void jes_throw_error(jes_err_t e){
    TaskHandle_t hj = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(hj);
    return __core_error_throw(e, pj);
}


void jes_notify_job(const char* name, void* notification){
    __job_notify_generic(__job_get_job_by_name(name), notification, 0);
}


void jes_notify_job_ISR(const char* name, void* notification){
    __job_notify_generic(__job_get_job_by_name(name), notification, 1);
}


void* jes_wait_for_notification(void){
    return __job_sleep_until_notified_generic();
}