#include "core.h"
#include "cli.h"

static core_t core;

err_t core_init(){
    core.state = e_state_init;
    err_t stat;
    stat = core_register_job("core", 2048, 1, core_job);
    if(stat != e_no_err){ return stat; }
    stat = core_register_job("initcli", 2048, 1, init_cli);
    if(stat != e_no_err){ return stat; }

    stat = core_launch_job("core");
    if(stat != e_no_err){ return stat; }
    stat = core_launch_job("initcli");
    if(stat != e_no_err){ return stat; }
    core.state = e_state_idle;
    return e_no_err;
}

err_t core_register_job(const char* n, 
                        uint32_t m,
                        uint8_t p, 
                        void (*f)(void* p)){
    return __register_job(&core.job_list, n, m, p, f);
}


job_struct_t* core_get_job(const char* n){
    return __get_job(&core.job_list, n);
}


err_t core_launch_job(const char* n){
    return __launch_job(&core.job_list, n);
}


void core_job(void* p){
    while(true){
        job_struct_t* pj = sleep_until_notified();
        core.state = e_state_spawning;
        core_launch_job(pj->name);
        core.state = e_state_idle;
    }
}
