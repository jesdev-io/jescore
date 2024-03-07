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


jes_err_t register_and_launch_job(const char* name,
                                  uint32_t mem_size,
                                  uint8_t priority,
                                  void (*function)(void* p),
                                  bool is_loop){
    jes_err_t stat = __job_register_job(name,
                                        mem_size,
                                        priority,
                                        function,
                                        is_loop,
                                        e_role_user);
    if(stat != e_err_no_err){ return stat; }
    return __job_launch_job_by_name(name, e_origin_api);
}


jes_err_t jesprint(const char* s){
    return __base_job_echo_wrapper(s, e_origin_api);
}


jes_err_t set_args(char* s, job_struct_t* pj){
    return __job_set_args(s, pj);
}


char* get_args(job_struct_t* pj){
    return __job_get_args(pj);
}


jes_err_t set_param(void* p, job_struct_t* pj){
    return __job_set_param(p, pj);
}


void* get_param(job_struct_t* pj){
    return __job_get_param(pj);
}