#include "base_jobs.h"
#include "job_driver.h"
#include "uart_unif.h"
#include "string.h"
#include "job_names.h"
#include "core.h"
#include "cli.h"


void __base_job_echo(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    uart_unif_write((char*)pj->args);
}


void __base_job_help(void* p){
    char desc[__MAX_JOB_ARGS_LEN_BYTE*2] = {0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    sprintf(desc, "\x1b[1mAvailable jobs:\x1b[0m\n\r");
    uart_unif_write(desc);
    while(cur != NULL){
        if(cur->role == e_role_base){
            sprintf(desc, "\t- (base) %s\n\r", cur->name);
            uart_unif_write(desc);
        }
        else if(cur->role == e_role_user){
            sprintf(desc, "\t- (user) %s\n\r", cur->name);
            uart_unif_write(desc);
        }
        cur = cur->pn;
    }
}


void __base_job_stats(void* p){
    job_struct_t* pj = (job_struct_t*)p;

    uint8_t flag_none = 0;  // just print user jobs
    uint8_t flag_a = 0;     // print user and base jobs
    
    if(pj->args[0] == 0) flag_none = 1;
    else if(strcmp(pj->args, "-a") == 0) flag_a = 1;
    else if(strcmp(pj->args, "-aa") == 0); // no filtering; every iteration is printed.
    else{
        char msg[__MAX_JOB_ARGS_LEN_BYTE*2];
        sprintf(msg, "Unknown specifier <%s>.\n\r", pj->args);
        uart_unif_write(msg);
        pj->error = e_err_param;
        return;
    }
    
    char desc[__MAX_JOB_ARGS_LEN_BYTE*2] = {0};
    char header[__MAX_JOB_ARGS_LEN_BYTE*3] = {0};
    char spacing_name[] = {'\t', 0, 0};
    char spacing_addr[] = {'\t', 0, 0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    sprintf(desc, "\x1b[1mname\t\thandle\t\tmemory\tprio\tloop\tinstances\terror\x1b[0m\n\r");
    if(!flag_none){
        sprintf(header, "%sjescore%s running on %s%s%s (FW %s)\n\r\n\r", 
            CLR_Y, CLR_X, CLR_G, BUILD_PLATFORM_NAME, CLR_X, JES_FW_VER);
        uart_unif_write(header);
    }
    uart_unif_write(desc);
    uint8_t* clr;

    while(cur != NULL){
        if ((flag_none && (cur->role == e_role_core || cur->role == e_role_base)) || 
            (flag_a && cur->role == e_role_core)) {
            cur = cur->pn;
            continue;
        }

        spacing_name[1] = (strlen(cur->name) < 8) ? '\t' : 0;
        spacing_addr[1] = (cur->handle == NULL) ? '\t' : 0;

        switch (cur->role) {
            case e_role_core: clr = CLR_Gr; break;
            case e_role_base: clr = CLR_Y;  break;
            case e_role_user: clr = CLR_G;  break;
            default:          clr = CLR_X;  break;
        }
        sprintf(desc, "%s%s%s%lx%s%ld\t%d\t%d\t%d\t\t%d%s\n\r", 
                clr,
                cur->name, 
                spacing_name,
                (uint32_t)cur->handle, 
                spacing_addr,
                cur->mem_size, 
                cur->priority,
                cur->is_loop,
                cur->instances,
                cur->error,
                CLR_X);
        uart_unif_write(desc);
        cur = cur->pn;
    }
}


void __base_job_bench(void* p){
    job_struct_t* pj_self = (job_struct_t*)p;
    if(strcmp(pj_self->args, "") == 0){
        uart_unif_write("Provide a job name to bench!\n\r");
        return;
    }
    char msg[80];
    job_struct_t* pj_arg = __job_get_job_by_name(pj_self->args);
    if(!pj_arg){
        sprintf(msg, "Unknown job <%s> to benchmark!\n\r", pj_self->args);
        uart_unif_write(msg);
        return;
    }
    if(pj_arg->timing_end == 0 && !pj_arg->is_loop){
        sprintf(msg, "Job <%s> has not yet run once!\n\r", pj_arg->name);
        uart_unif_write(msg);
        return;
    }
    if(pj_arg->timing_end == 0 && pj_arg->is_loop){
        sprintf(msg, "Job <%s> is a loop without timing hooks!\n\r", pj_arg->name);
        uart_unif_write(msg);
        return;
    }
    char lesser[] = "<";
    uint32_t timing = __job_get_timing(pj_arg);
    if(timing) lesser[0] = '\0';
    sprintf(msg, "Roundtrip time (%s) = [ %s%ld ] ms\n\r", pj_arg->name, lesser, ++timing);
    uart_unif_write(msg);
}