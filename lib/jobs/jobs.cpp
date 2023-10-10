#include "jobs.h"

err_t __register_job(job_struct_t** job_list,
                   const char* n, 
                   uint32_t m,
                   uint8_t p, 
                   void (*f)(void* p)){
    if(n == NULL || f == NULL){
        return e_is_zero;
    }
    if(m == 0 || p == 0){
        return e_is_zero;
    }
    job_struct_t* pj = (job_struct_t*)malloc(sizeof(job_struct_t));
    err_t stat = __copy_name(pj->name, (char*)n);
    if(stat != e_no_err){ return stat; }
    pj->handle = NULL;
    pj->mem_size = m;
    pj->priority = p;
    pj->function = f;
    pj->pn = *job_list;
    *job_list = pj;
    return e_no_err;
}


job_struct_t* __get_job(job_struct_t** job_list, char* n){
    job_struct_t* cur = *job_list;
    while(cur != NULL){
        if(strcmp((cur)->name, n) == 0){ 
            return cur; }
        cur = cur->pn;
    }
    return NULL;
}


err_t __launch_job(job_struct_t** job_list, char* n){
    BaseType_t stat;
    job_struct_t* pj = __get_job(job_list, n);
    if(pj == NULL){ return e_mem_null; }
    stat = xTaskCreate(pj->function,
                pj->name,
                pj->mem_size,
                (void*)job_list,
                pj->priority,
                &pj->handle);
    if(stat != pdPASS){ return e_mem_null; }
    return e_no_err;
}


static err_t __copy_name(char* buf, char* n){
    if(buf == NULL || n == NULL){ return e_is_zero; }
    uint8_t i = 0;
    while(n[i] != '\0'){
        if(++i == MAX_JOB_NAME_LEN_BYTE){ return e_too_long; }
    }
    strcpy(buf, n);
    return e_no_err;
}


void job_notify(job_struct_t* pjob_to_notify, 
                job_struct_t* pjob_to_run, 
                bool from_isr){
    if(from_isr){
        BaseType_t dummy = pdFALSE;
        pjob_to_run->caller = e_origin_interrupt;
        xTaskNotifyFromISR(pjob_to_notify->handle, 
                           (uint32_t)pjob_to_run, 
                           eSetValueWithOverwrite, 
                           &dummy);
    }
    else{
        xTaskNotify(pjob_to_notify->handle,
                    (uint32_t)pjob_to_run,
                    eSetValueWithOverwrite);
    }
}


job_struct_t* job_notify_take(TickType_t ticks_to_wait){
    uint32_t raw = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
    job_struct_t* pjob_to_run = (job_struct_t*)raw;
    return pjob_to_run;
}


job_struct_t* sleep_until_notified(){
    job_struct_t* pjob_to_run = job_notify_take(portMAX_DELAY);
    return pjob_to_run;
}