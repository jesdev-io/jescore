#include "base_jobs.h"
#include "job_driver.h"
#include "print_utils.h"


void __base_job_echo(void* p){
    job_struct_t* pj = __job_get_self((job_struct_t**)p, __base_job_echo);
    __cli_output_serial(pj->args);
    vTaskDelete(NULL);
}


