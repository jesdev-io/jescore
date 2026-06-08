#include "jescore_api.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"
#include "base_jobs.h"
#include "delay_unif.h"

static uint8_t api_initialized = 0;
static uint8_t sched_run = 0;


jes_err_t jes_init(void){
    jes_err_t je = __core_init();
    if(je != e_err_no_err) return je;
    api_initialized = 1;
    #ifdef BUILD_FOR_ESP32
    sched_run = 1;
    #endif // BUILD_FOR_ESP32
    return e_err_no_err;
}

#ifdef BUILD_FOR_STM32

void jes_dispatch(void){
    sched_run = 1;
    vTaskStartScheduler();
}

#endif // BUILD_FOR_STM32


uint8_t jes_is_init(void){
    return api_initialized;
}


jes_err_t jes_register_job(const char* name,
                       uint32_t mem_size,
                       uint8_t priority,
                       void (*function)(void* p),
                       uint8_t is_loop,
                       uint8_t is_singleton){
    if(!api_initialized) return e_err_uninitialized;
    return __job_register_job(name,
                              mem_size,
                              priority,
                              function,
                              is_loop,
                              is_singleton,
                              e_role_user);
    }


jes_err_t jes_launch_job(const char* name){
    if(!api_initialized) return e_err_uninitialized;
    if(!sched_run) return __job_launch_job_by_name(name, e_origin_api);
    return __job_launch_job_by_name_args_core(name, e_origin_api, "");
}


jes_err_t jes_launch_job_args(const char* name, const char* args){
    if(!api_initialized) return e_err_uninitialized;
    if(!sched_run){
        job_struct_t* pj = __job_get_job_by_name(name);
        if (pj == NULL) { return e_err_unknown_job; }
        jes_err_t e = __job_set_args((char*)args, pj);
        if (e != e_err_no_err) { return e; }
        return __job_launch_job_by_name(name, e_origin_api);
    } 
    return __job_launch_job_by_name_args_core(name, e_origin_api, args);
}


jes_err_t jes_register_and_launch_job(const char* name,
                                  uint32_t mem_size,
                                  uint8_t priority,
                                  void (*function)(void* p),
                                  uint8_t is_loop,
                                  uint8_t is_singleton){
    if(!api_initialized) return e_err_uninitialized;
    jes_err_t stat = __job_register_job(name,
                                        mem_size,
                                        priority,
                                        function,
                                        is_loop,
                                        is_singleton,
                                        e_role_user);
    if(stat != e_err_no_err){ return stat; }
    return jes_launch_job(name);
}


jes_err_t jes_unregister_job(const char* name){
    if(!api_initialized) return e_err_uninitialized;
    return __job_unregister_job(name);
}


jes_err_t jes_job_set_args(const char* s){
    if(!api_initialized || !sched_run) return e_err_uninitialized;
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_args(s, pj);
}


char* jes_job_get_args(void){
    if(!api_initialized || !sched_run) return NULL;
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_args(pj);
}


char* jes_job_arg_next(void) {
    if(!api_initialized || !sched_run) return NULL;
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


uint8_t jes_job_is_arg(const char* arg, const char* name){
    return strcmp((const char*)arg, name) == 0;
}


jes_err_t jes_job_set_param(const void* p){
    if(!api_initialized || !sched_run) return e_err_uninitialized;
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return e_err_is_zero; }
    return __job_set_param(p, pj);
}


void* jes_job_get_param(void){
    if(!api_initialized || !sched_run) return NULL;
    TaskHandle_t caller = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(caller);
    if (pj == NULL) { return NULL; }
    return __job_get_param(pj);
}


jes_err_t jes_error_get(const char* job_name){
    if(!api_initialized) return e_err_uninitialized;
    return __core_error_get(job_name);
}


jes_err_t jes_error_get_any(void){
    if(!api_initialized) return e_err_uninitialized;
    return __core_error_get_any();
}


void jes_throw_error(jes_err_t e){
    if(!api_initialized || !sched_run) return;
    TaskHandle_t hj = xTaskGetCurrentTaskHandle();
    job_struct_t* pj = __job_get_job_by_handle(hj);
    __core_error_throw(e, pj);
}


jes_err_t jes_notify_job(const char* name, const void* notification){
    if(!api_initialized || !sched_run) return e_err_uninitialized;
    job_struct_t* pj = __job_get_job_by_name(name);
    if (pj == NULL) { return e_err_unknown_job; }
    return __job_notify_generic(pj, (void*)notification, 0);
}


jes_err_t jes_notify_job_ISR(const char* name, const void* notification){
    if(!api_initialized || !sched_run) return e_err_uninitialized;
    job_struct_t* pj = __job_get_job_by_name(name);
    if (pj == NULL) { return e_err_unknown_job; }
    return __job_notify_generic(pj, (void*)notification, 1);
}


void* jes_wait_for_notification(void){
    if(!api_initialized || !sched_run) return NULL;
    return __job_sleep_until_notified_generic();
}

void jes_delay_job_ms(uint32_t ms){
    __delay_job_ms(ms);
}