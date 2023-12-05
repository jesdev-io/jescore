#include "base_jobs.h"
#include "job_driver.h"
#include "uart.h"
#include "string.h"



void __base_job_echo(void* p){
    job_struct_t* pj = __job_get_job_by_func(__base_job_echo);
    uint8_t* msg = (uint8_t*)(pj->args);
    char buf[64] = {0};
    sprintf(buf, "%s\n", pj->args);
    uart_unif_write((uint8_t*)buf, strlen(buf));
}


