#include "core.h"
#include "cli.h"

static core_t core;

err_t core_init(){
    core.state = e_state_init;
    err_t stat;
    stat = core_register_job("core", 2048, 1, core_job);
    if(stat != e_no_err){ return stat; }
    stat = core_register_job("CLI", 4096, 1, processInput);
    if(stat != e_no_err){ return stat; }

    stat = core_launch_job("core");
    if(stat != e_no_err){ return stat; }
    stat = core_launch_job("CLI");
    if(stat != e_no_err){ return stat; }
    core.state = e_state_idle;
    return e_no_err;
}

err_t core_register_job(const char* n, 
                        uint32_t m,
                        uint8_t p, 
                        void (*f)(void* p)){
    return __register_job(&core.base_job, n, m, p, f);
}


job_struct_t* core_get_job(char* n){
    return __get_job(&core.base_job, n);
}


err_t core_launch_job(char* n){
    return __launch_job(&core.base_job, n);
}


err_t core_launch_job_from_cli(char* n, char* args){
    return __launch_job_from_cli(&core.base_job, n, args);
}


void core_job(void* p){
    while(true){
        cmd_struct_t c = sleep_until_notified();
    }
}