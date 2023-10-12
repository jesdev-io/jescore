#include "jescore.h"
#include "core.h"
#include "job_driver.h"


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
    return __core_launch_job(name);
}