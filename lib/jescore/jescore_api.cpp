#include "jescore_api.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"


jes_err_t jes_init(){
    return __core_init();
}


jes_err_t register_job(const char* name,
                   uint32_t mem_size,
                   uint8_t priority,
                   void (*function)(void* p),
                   bool is_loop){
    return __job_register_job(name,
                               mem_size,
                               priority,
                               function,
                               is_loop);
    }


jes_err_t launch_job(const char* name){
    return __job_launch_job_by_name(name);
}


jes_err_t to_printer(const char* s){
    job_struct_t* printer = __job_get_job_by_name(PRINT_JOB_NAME);
    job_struct_t* core_job = __job_get_job_by_name(CORE_JOB_NAME);
    jes_err_t stat = __job_copy_name(printer->args, (char*)s);
    if(stat != e_err_no_err){ return stat; }
    printer->caller = e_origin_api;
    __job_notify(core_job, printer, false);
    return e_err_no_err;
}