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

/// @brief FreeRTOS task priority for the jescore dispatcher task.
/// @note FreeRTOS priority numbers are ascending: 0 is the idle/lowest priority,
///       and larger numbers are higher priority. Keep the core above low-priority
///       background work so CLI/API dispatch cannot starve, but below application
///       real-time/control tasks so jescore does not dominate user scheduling.
#ifndef JES_CORE_TASK_PRIORITY
#define JES_CORE_TASK_PRIORITY 2
#endif
#if JES_CORE_TASK_PRIORITY == 0
#warning "JES_CORE_TASK_PRIORITY is 0; CLI/API dispatch can starve behind any ready application task."
#endif
#if defined(configMAX_PRIORITIES) && (JES_CORE_TASK_PRIORITY >= configMAX_PRIORITIES)
#error "JES_CORE_TASK_PRIORITY must be less than configMAX_PRIORITIES."
#endif

/// @brief Timeout for core job notification wait (ms).
/// @note This bounds the core task's idle wait when no jobs are pending. It does
///       not determine dispatch latency once a notification is queued; task priority
///       and scheduler availability do.
#ifndef JES_CORE_NOTIFY_TIMEOUT_MS
#define JES_CORE_NOTIFY_TIMEOUT_MS 100
#endif

/// @brief Get the core's lock semaphore.
/// @return Pointer to the core's lock semaphore.
/// @note This allows other modules to synchronize access to core data structures.
SemaphoreHandle_t __core_get_lock(void);

/// @brief Macro for atomic access to core data structures.
/// @param code Block of code to execute with core.lock held.
/// @note This macro acquires core.lock before executing the code block
///       and releases it afterwards, creating an atomic context for the operation.
#define WITH_CORE_LOCK(code) \
    do { \
        SemaphoreHandle_t _lock = __core_get_lock(); \
        xSemaphoreTake(_lock, portMAX_DELAY); \
        code \
        xSemaphoreGive(_lock); \
    } while(0)

#if __JES_LOG_LEN > 0
#define JES_LOG_FAULT(pj) __core_add_to_log_auto((job_struct_t*)pj, "\x1b[31m" "fault" "\x1b[0m")
#define JES_LOG_REGISTER(pj) __core_add_to_log_auto((job_struct_t*)pj, "rgistr")
#define JES_LOG_LAUNCH(pj) __core_add_to_log_auto((job_struct_t*)pj, "launch")
#define JES_LOG_FINISH(pj) __core_add_to_log_auto((job_struct_t*)pj, "finish")
#else
#define JES_LOG_FAULT(pj) 
#define JES_LOG_REGISTER(pj) 
#define JES_LOG_LAUNCH(pj) 
#define JES_LOG_FINISH(pj)
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
/// @brief Log entry struct. Contains printable information about jobs.
typedef struct log_entry_t{
    uint32_t sys_time;
    char type[JES_LOG_TYPE_NAME_LEN];
    char name[__MAX_JOB_NAME_LEN_BYTE];
    char args[__MAX_JOB_ARGS_LEN_BYTE];
    uint8_t instances;
    e_role_t role;
    jes_err_t error;
} log_entry_t;
#endif


/// @brief Main core object, has one instance.
/// @param state (state_t): current state of core (fsm).
/// @param job_list (job_struct_t*): pointers to job functions in array format.
typedef struct core_t{
    state_t state;
    job_struct_t* job_list;
    SemaphoreHandle_t lock;
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