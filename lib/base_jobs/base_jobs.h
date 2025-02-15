#ifndef _BASE_JOBS_H_
#define _BASE_JOBS_H_

#include "commands.h"
#include "jes_err.h"

void __base_job_echo(void* p);

jes_err_t __base_job_echo_wrapper(const char* s, origin_t o);

void __base_job_help(void* p);

void __base_job_stats(void* p);

#endif