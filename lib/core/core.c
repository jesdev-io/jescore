#include "core.h"
#include "cli.h"
#include "base_jobs.h"
#include "job_names.h"
#include "delay_unif.h"
#include "uart_unif.h"
#include "jescore_api.h"

static core_t core = {
    .state = e_state_init,
    .job_list = NULL,
    #if __JES_LOG_LEN > 0
    .log_write = 0,
    .log_read = 0,
    #endif
};

jes_err_t __core_init(){
    if(core.state == e_state_idle) return e_err_no_err;
    jes_err_t e;
    e = __job_register_job(CORE_JOB_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __core_job, 1, e_role_core);
    if(e != e_err_no_err){ return e; }
    e = __job_register_job(ERROR_HANDLER_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __core_job_err_handler, 0, e_role_core);
    if(e != e_err_no_err){ return e; }

    #ifndef JES_DISABLE_CLI
    #if __JES_LOG_LEN > 0
    e = __job_register_job(LOG_PRINTER_NAME, BOARD_MIN_JOB_HEAP_MEM, 1, __core_log_printer, 0, e_role_base);
    #endif // __JES_LOG_LEN > 0
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
    sprintf(err_print_job->args, "%s (%d)", description, e);
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


jes_err_t __core_error_get(const char* job_name){
    job_struct_t* pj = __job_get_job_by_name(job_name);
    if (pj == NULL) { return e_err_unknown_job; }
    return pj->error;
}


jes_err_t __core_error_get_any(){
    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;
    jes_err_t e;
    while(cur != NULL){
        e = cur->error;
        if(e != e_err_no_err) { return e; }
        cur = cur->pn;
    }
    return e_err_no_err;
}


void __core_error_throw(jes_err_t e, job_struct_t* pj){
    if (pj == NULL) { return; }
    pj->error = e;
}


#if __JES_LOG_LEN > 0
void __core_add_to_log_index(job_struct_t* pj, uint32_t idx, const char* type){
    core.log[idx].sys_time = __get_systime_ms();
    strncpy(core.log[idx].type, type, JES_LOG_TYPE_NAME_LEN);
    if(pj == NULL){
        memset(&core.log[idx].job_state, 0, sizeof(job_struct_t));
    }
    else{
        memcpy(&core.log[idx].job_state, pj, sizeof(job_struct_t));
    }
}


void __core_add_to_log_auto(job_struct_t* pj, const char* type){
    __core_add_to_log_index(pj, core.log_write, type);
    core.log_read = core.log_write;
    if(++core.log_write == __JES_LOG_LEN){
        core.log_write = 0;
    }
}

log_entry_t __core_read_from_log_next(void){
    log_entry_t le = {
        .sys_time = core.log[core.log_read].sys_time,
        .job_state = core.log[core.log_read].job_state,
    };
    strcpy(le.type, core.log[core.log_read].type);
    if(++core.log_read == __JES_LOG_LEN){
        core.log_read = 0;
    }
    return le;
}

#ifndef JES_DISABLE_CLI
void __core_log_printer(void* p){
    log_entry_t le = __core_read_from_log_next();
    char desc[__MAX_JOB_ARGS_LEN_BYTE*4] = {0};
    char header[] = "systime (ms) type\t name\t\tinstances\terror\targs\n\r";
    uint8_t* clr;
    uart_unif_write(header);
    for(uint32_t i = 0; i < __JES_LOG_LEN; i++){
        if(le.type == NULL){
            continue;
        }
        le = __core_read_from_log_next();

        switch (le.job_state.role) {
            case e_role_core: clr = CLR_Gr; break;
            case e_role_base: clr = CLR_Y;  break;
            case e_role_user: clr = CLR_G;  break;
            default:          clr = CLR_X;  break;
        }
        sprintf(desc, "(%010ld) %s:\t %s%s\t\t%d\t\t%d\t%s%s\n\r", 
                le.sys_time,
                le.type,
                clr,
                le.job_state.name,
                le.job_state.instances,
                le.job_state.error,
                le.job_state.args,
                CLR_X);
        uart_unif_write(desc);
    }
}
#endif // JES_DISABLE_CLI
#endif // __JES_LOG_LEN > 0

void __core_job(void* p){
    while(1){
        job_struct_t* pj = __job_sleep_until_notified_with_job();
        if(pj == NULL){
            core.state = e_state_fault;
            jes_err_t e = e_err_unknown_job;
            job_struct_t placeholder;
            memset(&placeholder, 0, sizeof(job_struct_t));
            strcpy(placeholder.name, "<?>");
            placeholder.error = e_err_unknown_job;
            JES_LOG_FAULT(&placeholder);
            __core_err_handler_inline(e, NULL);
        }
        else{
            core.state = e_state_spawning;
            pj->error = __job_launch_job_by_name(pj->name, pj->caller);
            if(pj->error != e_err_no_err){
                core.state = e_state_fault;
                __core_err_handler_inline(pj->error, NULL);
                JES_LOG_FAULT(pj);
            }
        }
        core.state = e_state_idle;
    }
}
