#include "job_driver.h"
#include "core.h"
#include "core_job_names.h"
#include "cli.h"
#include "delay_unif.h"
#include <malloc.h>

jes_err_t __job_register_job(const char* n, 
                         uint32_t m,
                         uint8_t p, 
                         void (*f)(void* p),
                         uint8_t is_loop,
                         e_role_t role){
    if(n == NULL || f == NULL){
        return e_err_is_zero;
    }
    if(m == 0 || p == 0){
        return e_err_is_zero;
    }
    job_struct_t** job_list = __core_get_job_list();
    if(__job_get_job_by_name(n) != NULL){
        return e_err_duplicate;
    }
    job_struct_t* pj = (job_struct_t*)malloc(sizeof(job_struct_t));
    jes_err_t stat = __job_copy_str(pj->name, (char*)n, __MAX_JOB_NAME_LEN_BYTE);
    if(stat != e_err_no_err){ return stat; }
    pj->handle = NULL;
    pj->mem_size = m;
    pj->priority = p;
    pj->function = f;
    memset(pj->args, 0, __MAX_JOB_ARGS_LEN_BYTE);
    pj->is_loop = is_loop;
    pj->instances = 0;
    pj->role = role;
    pj->caller = e_origin_undefined;
    pj->param = NULL;
    pj->error = e_err_no_err;
    pj->notif_queue = xQueueCreate(MAX_JOB_NOTIF_QUEUE_SIZE, sizeof(void*));
    if(pj->notif_queue == NULL) { return e_err_mem_null; }
    pj->pn = *job_list;
    *job_list = pj;
    return e_err_no_err;
}


job_struct_t* __job_get_job_by_name(const char* n){
    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;
    while(cur != NULL){
        if(strcmp((cur)->name, n) == 0){ 
            return cur; }
        cur = cur->pn;
    }
    return NULL;
}


job_struct_t* __job_get_job_by_func(void (*f)(void* p)){
    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;
    while(cur != NULL){
        if(cur->function == f){ 
            return cur; }
        cur = cur->pn;
    }
    return NULL;
}


job_struct_t* __job_get_job_by_handle(TaskHandle_t t){
    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;
    while(cur != NULL){
        if(cur->handle == t){ 
            return cur; }
        cur = cur->pn;
    }
    return NULL;
}


jes_err_t __job_launch_job(job_struct_t* pj, origin_t o){
    BaseType_t stat;
    pj->caller = o;
    if(pj == NULL){ return e_err_unknown_job; }
    if((o == e_origin_cli || o == e_origin_api) && pj->role == e_role_core){
        // Prohibit the calling of core jobs from API and CLI
        return e_err_prohibited;
    }
    stat = xTaskCreate(__job_runtime_env,
                pj->name,
                pj->mem_size,
                (void*)pj,
                pj->priority,
                &pj->handle);
    if(stat != pdPASS){ return e_err_mem_null; }
    return e_err_no_err;
}


jes_err_t __job_launch_job_by_name(const char* n, origin_t o){
    job_struct_t* pj = __job_get_job_by_name(n);
    return __job_launch_job(pj, o);
}


jes_err_t __job_launch_job_by_name_args(const char* n, origin_t o, const char* args){
    if (args == NULL) { return e_err_is_zero; }
    job_struct_t* pj = __job_get_job_by_name(n);
    if (pj == NULL) { return e_err_is_zero; }
    jes_err_t e = __job_set_args((char*)args, pj);
    if (e != e_err_no_err) { return e; }
    return __job_launch_job(pj, o);
}


void __job_runtime_env(void* p){
    volatile job_struct_t* pj = (job_struct_t*)p;

    #ifndef JES_DISABLE_CLI
    /* This section reprints the CLI prefix in case the started job is a loop.
     * This is useful when an infinite job is triggered once by the CLI, otherwise
     * the prefix would never return.
    */
    if(pj->is_loop && __cli_get_sess_state() == 1){
        __cli_set_sess_state(0);
        __cli_reprint_header();
    }
    #endif

    pj->instances++;
    
    /// This runs the user function
    pj->function((void*)pj);
    /// ---------------------------

    pj->instances--;

    #ifndef JES_DISABLE_CLI
    /* This section reprints the CLI prefix in case the started job is done.
     * This is the opposite of the similar looking statement above.
    */
    if(!pj->is_loop && __cli_get_sess_state() == 1){
        __cli_set_sess_state(0);
        jes_delay_job_ms(50);
        __cli_reprint_header();
    }
    #endif
    pj->handle = NULL;
    vTaskDelete(NULL);
}


jes_err_t __job_copy_str(char* buf, char* str, uint16_t max_len){
    if(buf == NULL || str == NULL){ return e_err_is_zero; }
    uint8_t i = 0;
    while(str[i] != '\0'){
        if(++i == max_len){ return e_err_too_long; }
    }
    strcpy(buf, str);
    return e_err_no_err;
}


void __job_notify_generic(job_struct_t* pjob_to_notify, 
                          void* notif, 
                          uint8_t from_isr){
    if(from_isr){
        BaseType_t dummy = pdFALSE;
        xTaskNotifyFromISR(pjob_to_notify->handle, 
                           (uint32_t)notif, 
                           eSetValueWithOverwrite, 
                           &dummy);
    }
    else{
        xTaskNotify(pjob_to_notify->handle,
                    (uint32_t)notif,
                    eSetValueWithOverwrite);
    }
}


void __job_notify_with_job(job_struct_t* pjob_to_notify, 
                job_struct_t* pjob_to_run, 
                uint8_t from_isr){
    __job_notify_generic(pjob_to_notify, pjob_to_run, from_isr);
}


void* __job_notify_generic_take(TickType_t ticks_to_wait){
    uint32_t raw = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
    return (void*)raw;
}


job_struct_t* __job_notify_with_job_take(TickType_t ticks_to_wait){
    uint32_t raw = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
    job_struct_t* pjob_to_run = (job_struct_t*)raw;
    return pjob_to_run;
}


void* __job_sleep_until_notified_generic(void){
    return __job_notify_generic_take(portMAX_DELAY);
}


job_struct_t* __job_sleep_until_notified_with_job(void){
    job_struct_t* pjob_to_run = __job_notify_with_job_take(portMAX_DELAY);
    return pjob_to_run;
}


jes_err_t __job_set_args(char* s, job_struct_t* pj){
    return __job_copy_str(pj->args, s, __MAX_JOB_ARGS_LEN_BYTE);
}


char* __job_get_args(job_struct_t* pj){
    if(pj == NULL){ return NULL; }
    return pj->args;
}


jes_err_t __job_set_param(void* p, job_struct_t* pj){
    if(pj == NULL){ return e_err_is_zero; }
    pj->param = p;
    return e_err_no_err;
}


void* __job_get_param(job_struct_t* pj){
    if(pj == NULL){ return NULL; }
    return pj->param;
}