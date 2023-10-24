#include "base_jobs.h"
#include "job_driver.h"



void __base_job_echo(void* p){
    job_struct_t* pj = __job_get_job_by_func(__base_job_echo);
    Serial.println(pj->args);
}


