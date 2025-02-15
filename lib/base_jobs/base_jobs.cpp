#include "base_jobs.h"
#include "job_driver.h"
#include "uart.h"
#include "string.h"
#include "job_names.h"
#include "core.h"


void __base_job_echo(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    uint8_t* msg = (uint8_t*)(pj->args);
    char buf[64] = {0};
    sprintf(buf, "%s\n", pj->args);
    uart_unif_write((uint8_t*)buf, strlen(buf));
}


void __base_job_help(void* p){
    char desc[__MAX_JOB_ARGS_LEN_BYTE*2] = {0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    sprintf(desc, "\x1b[1mAvailable jobs:\x1b[0m\n\r");
    uart_unif_write((uint8_t*)desc, strlen(desc));
    while(cur != NULL){
        if(cur->role == e_role_base){
            sprintf(desc, "\t\t(base) %s\n\r", cur->name);
            uart_unif_write((uint8_t*)desc, strlen(desc));
        }
        else if(cur->role == e_role_user){
            sprintf(desc, "\t\t(user) %s\n\r", cur->name);
            uart_unif_write((uint8_t*)desc, strlen(desc));
        }
        cur = cur->pn;
    }
}


void __base_job_stats(void* p){
    char desc[__MAX_JOB_ARGS_LEN_BYTE*2] = {0};
    char spacing_name[] = {'\t', 0, 0};
    char spacing_addr[] = {'\t', 0, 0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    sprintf(desc, "\x1b[1mname\t\thandle\t\tmemory\tprio\tloop\tinstances\x1b[0m\n\r");
    uart_unif_write((uint8_t*)desc, strlen(desc));
    while(cur != NULL){
        if(strlen(cur->name) < 8){
            spacing_name[1] = '\t';
        }
        else{
            spacing_name[1] = 0;
        }
        if(cur->handle == NULL){
            spacing_addr[1] = '\t';
        }
        else{
            spacing_addr[1] = 0;
        }
        sprintf(desc, "\t\t%s%s%x%s%d\t%d\t%d\t%d\n\r", 
                cur->name, 
                spacing_name,
                cur->handle, 
                spacing_addr,
                cur->mem_size, 
                cur->priority,
                cur->is_loop,
                cur->instances);
        uart_unif_write((uint8_t*)desc, strlen(desc));
        cur = cur->pn;
    }
}
