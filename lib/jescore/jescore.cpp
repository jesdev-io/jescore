#include "jescore.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"


err_t jes_init(){
    return __core_init();
}


err_t register_job(const char* name,
                   uint32_t mem_size,
                   uint8_t priority,
                   void (*function)(void* p)){
    return __core_register_job(name,
                               mem_size,
                               priority,
                               function);
    }


err_t launch_job(const char* name){
    return __core_launch_job_by_name(name);
}


err_t to_printer(const char* s){
    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* printer = __job_get_job_by_name(job_list, PRINT_JOB_NAME);
    job_struct_t* core_job = __job_get_job_by_name(job_list, CORE_JOB_NAME);
    uint8_t i = 0;
    while(s[i] != '\0' && i < __MAX_JOB_NAME_LEN_BYTE){
        printer->args[i] = s[i];
        i++;
    }
    if(i == __MAX_JOB_NAME_LEN_BYTE){
        return e_err_too_long;
    }
    printer->caller = e_origin_api;
    __job_notify(core_job, printer, false);
    return e_err_no_err;
}