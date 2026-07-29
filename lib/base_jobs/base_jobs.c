#include "base_jobs.h"
#include "job_driver.h"
#include "uart_unif.h"
#include "string.h"
#include "job_names.h"
#include "core.h"
#include "cli.h"


void __base_job_echo(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    uart_unif_writef_pfx(pj->name, "%s\n\r", (char*)pj->args);
}


void __base_job_help(void* p){
    job_struct_t* pj = (job_struct_t*)p;
    char desc[__MAX_JOB_ARGS_LEN_BYTE*2] = {0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    snprintf(desc, sizeof(desc), "\x1b[1mAvailable jobs:\x1b[0m\n\r");
    uart_unif_writef_pfx(pj->name, desc);
    while(cur != NULL){
        if(cur->role == e_role_base){
            snprintf(desc, sizeof(desc), "%s%-6s %s%s\n\r", CLR_Y, "base", cur->name, CLR_X);
            uart_unif_writef_pfx(pj->name, desc);
        }
        else if(cur->role == e_role_user){
            snprintf(desc, sizeof(desc), "%s%-6s %s%s\n\r", CLR_G, "user", cur->name, CLR_X);
            uart_unif_writef_pfx(pj->name, desc);
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
        snprintf(msg, sizeof(msg), "Unknown specifier <%s>.\n\r", pj->args);
        uart_unif_writef_pfx(pj->name, msg);
        pj->error = e_err_param;
        return;
    }
    
    char desc[__MAX_JOB_ARGS_LEN_BYTE*4] = {0};
    char header[__MAX_JOB_ARGS_LEN_BYTE*3] = {0};

    job_struct_t** job_list = __core_get_job_list();
    job_struct_t* cur = *job_list;

    snprintf(desc, sizeof(desc), "\x1b[1m%-16s %-12s %-6s %-4s %-4s %-9s %-5s\x1b[0m\n\r",
             "name", "handle", "memory", "prio", "loop", "instances", "error");
    if(!flag_none){
        snprintf(header, sizeof(header), "%sjescore%s running on %s%s%s (FW %s)\n\r\n\r", 
            CLR_Y, CLR_X, CLR_G, BUILD_PLATFORM_NAME, CLR_X, JES_FW_VER);
        uart_unif_writef_pfx(pj->name, header);
    }
    uart_unif_writef_pfx(pj->name, desc);
    uint8_t* clr; 

    while(cur != NULL){
        if ((flag_none && (cur->role == e_role_core || cur->role == e_role_base)) || 
            (flag_a && cur->role == e_role_core)) {
            cur = cur->pn;
            continue;
        }

        switch (cur->role) {
            case e_role_core: clr = CLR_Gr; break;
            case e_role_base: clr = CLR_Y;  break;
            case e_role_user: clr = CLR_G;  break;
            default:          clr = CLR_X;  break;
        }
        char singleton = 'x';
        char instances[8] = {0};
        if(cur->is_singleton) singleton = '1';
        snprintf(instances, sizeof(instances), "%u/%c", cur->instances, singleton);
        snprintf(desc, sizeof(desc), "%s%-16s%s %-12p %-6u %-4u %-4u %-9s %-5d\n\r", 
                clr,
                cur->name, 
                CLR_X,
                (void*)cur->handle, 
                cur->mem_size, 
                cur->priority,
                cur->is_loop,
                instances,
                cur->error);
        uart_unif_writef_pfx(pj->name, desc);
        cur = cur->pn;
    }
}


void __base_job_bench(void* p){
    job_struct_t* pj_self = (job_struct_t*)p;
    if(strcmp(pj_self->args, "") == 0){
        uart_unif_writef_pfx(pj_self->name, "Provide a job name to bench!\n\r");
        return;
    }
    job_struct_t* pj_arg = __job_get_job_by_name(pj_self->args);
    if(!pj_arg){
        uart_unif_writef_pfx(pj_self->name, "Unknown job <%s> to benchmark!\n\r", pj_self->args);
        return;
    }
    if(pj_arg->timing_end == 0 && !pj_arg->is_loop){
        uart_unif_writef_pfx(pj_self->name, "Job <%s> has not yet run once!\n\r", pj_arg->name);
        return;
    }
    if(pj_arg->timing_end == 0 && pj_arg->is_loop){
        uart_unif_writef_pfx(pj_self->name, "Job <%s> is a loop without timing hooks!\n\r", pj_arg->name);
        return;
    }
    char lesser[] = "<";
    uint32_t timing = __job_get_timing(pj_arg);
    if(timing) lesser[0] = '\0';
    uart_unif_writef_pfx(pj_self->name, "Roundtrip time (%s) = [ %s%ld ] ms\n\r", pj_arg->name, lesser, ++timing);
}