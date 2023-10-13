#include "core.h"
#include "cli.h"
#include "base_jobs.h"
#include "job_names.h"

static core_t core;

err_t __core_init(){
    core.state = e_state_init;
    err_t stat;
    stat = __core_register_job(CORE_JOB_NAME, 2048, 1, __core_job);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(PRINT_JOB_NAME, 4096, 1, __base_job_echo);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(ERROR_HANDLER_NAME, 1024, 1, __core_job_err_handler);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(INIT_CLI_JOB_NAME, 2048, 1, init_cli);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(HEADER_PRINTER_NAME, 1024, 2, reprint_header);
    if(stat != e_err_no_err){ return stat; }
    
    stat = __core_launch_job(CORE_JOB_NAME);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_launch_job(INIT_CLI_JOB_NAME);
    if(stat != e_err_no_err){ return stat; }
    core.state = e_state_idle;
    return e_err_no_err;
}

err_t __core_register_job(const char* n,
                        uint32_t m,
                        uint8_t p,
                        void (*f)(void* p)){
    return __job_register_job(&core.job_list, n, m, p, f);
}


job_struct_t* __core_get_job(const char* n){
    return __job_get_job(&core.job_list, n);
}


err_t __core_launch_job(const char* n){
    return __job_launch_job(&core.job_list, n);
}


static void __core_err_handler_inline(err_t e, void* args){

    job_struct_t* err_print_job = __core_get_job(PRINT_JOB_NAME);
    const char* description = NULL;
    switch (e)
    {
    case e_err_mem_null:
        description = "Memory could not be allocated!";
        break;
    case e_err_is_zero:
        description = "Result is unexpectedly 0!";
        break;
    case e_err_param:
        description = "Parameter error!";
        break;
    case e_err_peripheral_block:
        description = "Peripheral was blocked!";
        break;
    case e_err_core_fail:
        description = "Core failure!";
        break;
    case e_err_too_long:
        description = "Given string is too long!";
        break;
    case e_err_unknown_job:
        description = "Job has not been registered!";
        break;
    case e_err_leading_whitespace:
        description = "Leading whitespace error!";
        break;
    default:
        description = "Unknown error.";
        break;
    }
    strcpy(err_print_job->args, description);
    __core_launch_job(PRINT_JOB_NAME);
}


void __core_job_err_handler(void* p){
    job_struct_t* pj = __job_get_self((job_struct_t**)p, __core_job_err_handler);
    __core_err_handler_inline(pj->error, NULL);
    vTaskDelete(NULL);
}


job_struct_t** __core_get_job_list(void){
    return &core.job_list;
}


void __core_job(void* p){
    while(true){
        job_struct_t* pj = __job_sleep_until_notified();
        if(pj == NULL){
            core.state = e_state_fault;
            err_t e = e_err_unknown_job;
            __core_err_handler_inline(e, NULL);
        }
        else{
            core.state = e_state_spawning;
            err_t e = __core_launch_job(pj->name);
        }
        core.state = e_state_idle;
    }
}
