#include "core.h"
#include "cli.h"

static core_t core;

err_t __core_init(){
    core.state = e_state_init;
    err_t stat;
    stat = __core_register_job(CORE_JOB_NAME, 2048, 1, __core_job);
    if(stat != e_no_err){ return stat; }
    #ifndef JES_DISABLE_CLI
    stat = __core_register_job(INIT_CLI_JOB_NAME, 2048, 1, init_cli);
    if(stat != e_no_err){ return stat; }
    #endif

    stat = __core_launch_job(CORE_JOB_NAME);
    if(stat != e_no_err){ return stat; }
    #ifndef JES_DISABLE_CLI
    stat = __core_launch_job(INIT_CLI_JOB_NAME);
    if(stat != e_no_err){ return stat; }
    #endif
    core.state = e_state_idle;
    return e_no_err;
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


void __core_job(void* p){
    while(true){
        job_struct_t* pj = __job_sleep_until_notified();
        core.state = e_state_spawning;
        __core_launch_job(pj->name);
        core.state = e_state_idle;
    }
}
