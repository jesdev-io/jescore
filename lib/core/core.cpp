#include "core.h"
#include "cli.h"
#include "base_jobs.h"
#include "job_names.h"

static core_t core;

jes_err_t __core_init(){
    core.state = e_state_init;
    jes_err_t stat;
    stat = __job_register_job(CORE_JOB_NAME, 2048, 1, __core_job, true, e_role_core);
    if(stat != e_err_no_err){ return stat; }
    stat = __job_register_job(ERROR_HANDLER_NAME, 1024, 1, __core_job_err_handler, false, e_role_core);
    if(stat != e_err_no_err){ return stat; }

    #ifndef JES_DISABLE_CLI
    stat = __job_register_job(INIT_CLI_JOB_NAME, 2048, 1, init_cli, false, e_role_core);
    if(stat != e_err_no_err){ return stat; }
    stat = __job_register_job(HEADER_PRINTER_NAME, 1024, 1, reprint_header, false, e_role_core);
    if(stat != e_err_no_err){ return stat; }
    stat = __job_register_job(HELP_NAME, 2048, 1, __base_job_help, false, e_role_base);
    if(stat != e_err_no_err){ return stat; }
    stat = __job_register_job(STATS_NAME, 4096, 1, __base_job_stats, false, e_role_base);
    if(stat != e_err_no_err){ return stat; }
    #endif
    
    stat = __job_launch_job_by_name(CORE_JOB_NAME, e_origin_core);
    if(stat != e_err_no_err){ return stat; }

    #ifndef JES_DISABLE_CLI
    stat = __job_launch_job_by_name(INIT_CLI_JOB_NAME, e_origin_core);
    if(stat != e_err_no_err){ return stat; }
    #endif

    core.state = e_state_idle;
    return e_err_no_err;
}


static void __core_err_handler_inline(jes_err_t e, void* args){

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
    __job_launch_job_by_name(PRINT_JOB_NAME, e_origin_core);
}


void __core_job_err_handler(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    __core_err_handler_inline(pj->error, NULL);
}


job_struct_t** __core_get_job_list(void){
    return &core.job_list;
}


void __core_notify(job_struct_t* pjob_to_run, 
                    bool from_isr){
__job_notify(__job_get_job_by_name(CORE_JOB_NAME),
             pjob_to_run, from_isr);
}


void __core_job(void* p){
    while(true){
        job_struct_t* pj = __job_sleep_until_notified();
        if(pj == NULL){
            core.state = e_state_fault;
            jes_err_t e = e_err_unknown_job;
            __core_err_handler_inline(e, NULL);
        }
        else{
            core.state = e_state_spawning;
            jes_err_t e = __job_launch_job_by_name(pj->name, pj->caller);
        }
        core.state = e_state_idle;
    }
}
