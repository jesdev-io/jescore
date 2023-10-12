#include "core.h"
#include "cli.h"

static core_t core;

err_t __core_init(){
    core.state = e_state_init;
    err_t stat;
    stat = __core_register_job(CORE_JOB_NAME, 2048, 1, __core_job);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(PRINT_JOB_NAME, 4096, 1, __echo);
    if(stat != e_err_no_err){ return stat; }
    stat = __core_register_job(INIT_CLI_JOB_NAME, 2048, 1, init_cli);
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


void __err_handler(err_t e, void* args){

    job_struct_t* err_print_job = __core_get_job(PRINT_JOB_NAME);
    const char* description = NULL;
    switch (e)
    {
    case e_err_mem_null:
        description = "Memory could not be allocated!";
        break;
    case e_err_is_zero:
        description = "Result is 0 but was not supposed to be!";
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
    default:
        description = "Unknown error.";
        break;
    }
    strcpy(err_print_job->args, description);
    __core_launch_job(PRINT_JOB_NAME);
}


void __core_job(void* p){
    while(true){
        job_struct_t* pj = __job_sleep_until_notified();
        if(pj == NULL){
            core.state = e_state_fault;
            err_t e = e_err_unknown_job;
            __err_handler(e, (void*)pj);
        }
        else{
            core.state = e_state_spawning;
            err_t e = __core_launch_job(pj->name);
        }
        core.state = e_state_idle;
    }
}
