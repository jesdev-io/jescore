#ifndef _JESCORE_H_
#define _JESCORE_H_

/// @file Top API layer for user access.

#include "core.h"
#include "err.h"

/// @brief Start the core and all of its abilities.
/// @return Status. Returns `e_no_err` in case of successful launch.
err_t jes_init();


/// @brief Add a job (function block) to the list of all known jobs.
/// @param name: Name of job. Can't be longer than `MAX_JOB_NAME_LEN_BYTE`.
/// @param mem_size: Dynamic memory size for job.
/// @param priority: Priority of the job (1 is highest).
/// @param function: Function to run when the job is called. has to be of signature `void my_func(void* p)`.
/// @return Status. Returns `e_no_err` in case of successful launch.
err_t register_job(const char* name,
                   uint32_t mem_size,
                   uint8_t priority,
                   void (*function)(void* p));

#ifdef JES_DISABLE_CLI
/// @brief Start a registered job.
/// @param name: String name of job as set in `register_job()` @
/// @return Status. Returns `e_no_err` in case of successful launch.
err_t launch_job(const char* name);
#endif

#endif