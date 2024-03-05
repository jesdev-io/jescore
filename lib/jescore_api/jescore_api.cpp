#include "jescore_api.h"
#include "core.h"
#include "job_driver.h"
#include "job_names.h"
#include "base_jobs.h"


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
                               is_loop,
                               e_role_user);
    }


jes_err_t launch_job(const char* name){
    return __job_launch_job_by_name(name, e_origin_api);
}


jes_err_t jesprint(const char* s){
    return __base_job_echo_wrapper(s, e_origin_api);
}