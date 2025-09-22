#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CORE_H_
#define _CORE_H_

/// @file Main core acting as finite state machine.

#include "jes_err.h"
#include "commands.h"
#include "job_driver.h"

#ifndef JES_LOG_LEN
#define __JES_LOG_LEN 8
#else
#define __JES_LOG_LEN JES_LOG_LEN
#endif // JES_LOG_LEN

#define JES_LOG_TYPE_NAME_LEN 10

#if __JES_LOG_LEN > 0
#define JES_LOG_FAULT(pj) __core_add_to_log_auto((job_struct_t*)pj, "\x1b[31m" "fault" "\x1b[0m")
#define JES_LOG_REGISTER(pj) __core_add_to_log_auto((job_struct_t*)pj, "rgistr")
#define JES_LOG_LAUNCH(pj) __core_add_to_log_auto((job_struct_t*)pj, "launch")
#else
#define JES_LOG_FAULT(pj) 
#define JES_LOG_REGISTER(pj) 
#define JES_LOG_LAUNCH(pj) 
#endif // __JES_LOG_LEN > 0


/// @brief Types of states.
/// @note Always prefixed with "e_state".
typedef enum state_t{
    e_state_init,
    e_state_idle,
    e_state_fault,
    e_state_spawning,
    NUM_STATES
} state_t;

#if __JES_LOG_LEN > 0
typedef struct log_entry_t{
    uint32_t sys_time;
    char type[JES_LOG_TYPE_NAME_LEN];
    job_struct_t job_state;
} log_entry_t;
#endif


/// @brief Main core object, has one instance.
/// @param state (state_t): current state of core (fsm).
/// @param job_list (job_struct_t*): pointers to job functions in array format.
typedef struct core_t{
    state_t state;
    job_struct_t* job_list;
    #if __JES_LOG_LEN > 0
    log_entry_t log[__JES_LOG_LEN];
    uint32_t log_write;
    uint32_t log_read;
    #endif
}core_t;


/// @brief Core initializer.
/// @return status, `e_err_no_err` if OK.
/// @note Registers/launches itself and CLI if `JES_DISABLE_CLI` is not defined
jes_err_t __core_init();


/// @brief Core error handler.
/// @param e: Error to handle.
/// @param args: optional additional arguments.
void __core_err_handler_inline(jes_err_t e, void* args);


/// @brief Error handler as callable job
/// @param p: Mandatory args pointer.
void __core_job_err_handler(void* p);


/// @brief Get the main job list.
/// @return job list.
job_struct_t** __core_get_job_list();


/// @brief Notify the core with a job to do.
/// @param pjob_to_run: job to run.
/// @param from_isr: originating from an interrupt.
void __core_notify(job_struct_t* pjob_to_run, 
                    uint8_t from_isr);


/// @brief Get the error of a job.
/// @param job_name: job name (callable by CLI).
/// @return Error of the given job as stored by the core.
jes_err_t __core_error_get(const char* job_name);


/// @brief Get the first error that of all jobs.
/// @return Error of first job that has one.
/// @note Returns `e_err_no_err` in case that every
///       job is error-free.
jes_err_t __core_error_get_any();


/// @brief Actively throw an error and store it in the core.
/// @param e Error to throw.
void __core_error_throw(jes_err_t e, job_struct_t* pj);

#if __JES_LOG_LEN > 0

/// @brief 
/// @param pj 
/// @param idx 
void __core_add_to_log_index(job_struct_t* pj, uint32_t idx, const char* type);

/// @brief 
/// @param pj 
void __core_add_to_log_auto(job_struct_t* pj, const char* type);

/// @brief 
/// @param  
/// @return 
log_entry_t __core_read_from_log_next(void);

#ifndef JES_DISABLE_CLI
/// @brief 
/// @param p 
void __core_log_printer(void* p);
#endif

#endif // __JES_LOG_LEN > 0

/// @brief Main core job. Handles calls and runs jobs.
/// @param p: Mandatory args pointer.
void __core_job(void* p);

#endif

#ifdef __cplusplus
}
#endif