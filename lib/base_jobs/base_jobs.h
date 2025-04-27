#ifdef __cplusplus
extern "C" {
#endif

#ifndef _BASE_JOBS_H_
#define _BASE_JOBS_H_

#include "commands.h"
#include "jes_err.h"

void __base_job_echo(void* p);

void __base_job_help(void* p);

void __base_job_stats(void* p);

#endif

#ifdef __cplusplus
}
#endif