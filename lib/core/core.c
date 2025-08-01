#include "core.h"
#include "cli.h"
#include "base_jobs.h"
#include "job_names.h"

static core_t core;

jes_err_t __core_init(){
    core.state = e_state_init;
    jes_err_t e;
    e = __job_register_job(CORE_JOB_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __core_job, 1, e_role_core);
    if(e != e_err_no_err){ return e; }
    e = __job_register_job(ERROR_HANDLER_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __core_job_err_handler, 0, e_role_core);
    if(e != e_err_no_err){ return e; }

    #ifndef JES_DISABLE_CLI
    e = __cli_init();
    if(e != e_err_no_err){ return e; }
    e = __job_register_job(HELP_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __base_job_help, 0, e_role_base);
    if(e != e_err_no_err){ return e; }
    e = __job_register_job(STATS_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __base_job_stats, 0, e_role_base);
    if(e != e_err_no_err){ return e; }
    #endif
    
    e = __job_launch_job_by_name(CORE_JOB_NAME, e_origin_core);
    if(e != e_err_no_err){ return e; }

    core.state = e_state_idle;
    return e_err_no_err;
}


inline void __core_err_handler_inline(jes_err_t e, void* args){

    job_struct_t* err_print_job = __job_get_job_by_name(PRINT_JOB_NAME);
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
    case e_err_duplicate:
        description = "Entry already exists!";
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
    case e_err_prohibited:
        description = "Access denied!";
        break;
    default:
        description = "Unknown error.";
        break;
    }
    strcpy(err_print_job->args, description);
    __job_launch_job(err_print_job, e_origin_core);
}


void __core_job_err_handler(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    __core_err_handler_inline(pj->error, NULL);
}


job_struct_t** __core_get_job_list(void){
    return &core.job_list;
}


void __core_notify(job_struct_t* pjob_to_run, 
                    uint8_t from_isr){
__job_notify_with_job(__job_get_job_by_name(CORE_JOB_NAME),
             pjob_to_run, from_isr);
}


void __core_job(void* p){
    while(1){
        job_struct_t* pj = __job_sleep_until_notified_with_job();
        if(pj == NULL){
            core.state = e_state_fault;
            jes_err_t e = e_err_unknown_job;
            __core_err_handler_inline(e, NULL);
        }
        else{
            core.state = e_state_spawning;
            jes_err_t e = __job_launch_job_by_name(pj->name, pj->caller);
            if(e != e_err_no_err){
                core.state = e_state_fault;
                __core_err_handler_inline(e, NULL);
            }
            pj->error = e;
        }
        core.state = e_state_idle;
    }
}
