#ifndef _JOBS_H_
#define _JOBS_H_

/// @file Driver for jobs. Handles job registration, launching and inter-job communication.

#include <Arduino.h>
#include "err.h"
#include "commands.h"

#ifndef MAX_JOB_NAME_LEN_BYTE
#define __MAX_JOB_NAME_LEN_BYTE     16
#else
#define __MAX_JOB_NAME_LEN_LIMIT    32
#define __GET_SAFE_SIZE(SIZE, LIMIT) ((SIZE)<(LIMIT)?(SIZE):(LIMIT))
#define __MAX_JOB_NAME_LEN_BYTE __GET_SAFE_SIZE(MAX_JOB_NAME_LEN_BYTE, MAX_JOB_NAME_LEN_LIMIT)
#endif

/// @brief Abstraction struct for jobs to do.
/// @param name (char*): job name (callable by CLI).
/// @param handle (TaskHandle_t): handle to task (FreeRTOS).
/// @param mem_size (uint32_t): allocated stack size for task.
/// @param priority (uint8_t): priority of task.
/// @param function void(*)(void* p): function itself, function pointer.
/// @param args (void*): optional args obtained from CLI.
/// @param caller (origin_t): Requesting entity of job.
/// @param pn (job_struct_t*): pointer to next job (llist).
typedef struct job_struct_t{
    char name[__MAX_JOB_NAME_LEN_BYTE] = {0};
    TaskHandle_t handle = NULL;
    uint32_t mem_size = 0;
    uint8_t priority = 0;
    void (*function) (void* p) = NULL;
    char args[__MAX_JOB_NAME_LEN_BYTE] = {0};
    origin_t caller = e_origin_undefined;
    void* optional = NULL;
    struct job_struct_t* pn = NULL;
}job_struct_t;


/// @brief Job struct setter and list appender.
/// @param job_list: head of job-llist.
/// @param n: job name (callable by CLI).
/// @param m: allocated stack size for task.
/// @param p: priority of job.
/// @param f: function itself, function pointer.
/// @returns status, `e_no_err` if OK.
/// @note Checks for plausible task parameters,
/// e.g. forbids a mem size of 0 or empty
/// task function pointers.
err_t __job_register_job(job_struct_t** job_list,
                     const char* n, 
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p));


/// @brief Job getter based on name identifier.
/// @param job_list: head of job-llist.
/// @param n: job name (callable by CLI).
/// @returns job handle.
job_struct_t* __job_get_job(job_struct_t** job_list, const char* n);


/// @brief Job getter based on function signature.
/// @param job_list: head of job-llist.
/// @param f: function pointer of calling function.
/// @returns job handle.
job_struct_t* __job_get_self(job_struct_t** job_list, void (*f)(void* p));


/// @brief Job creation wrapper.
/// @param job_list: head of job-llist.
/// @param n: job name (callable by CLI).
/// @returns status, `e_no_err` if OK.
/// @note Checks if task memory could be allocated.
err_t __job_launch_job(job_struct_t** job_list, const char* n);


/// @brief Helper function for safety assertments around strcpy().
/// @param buf: name buffer (job_struct->name).
/// @param name: given name to copy (comes from CLI).
/// @returns status, `e_no_err` if OK.
static inline err_t __job_copy_name(char* buf, char* n);


/// @brief Task notification wrapper for FreeRTOS "xTaskNotify()".
/// @param pjob_to_notify: handle to job to be notified.
/// @param pjob_to_run: job that should be launched by the notified job.
/// @param from_isr: specifies ISR or non-ISR origin.
void __job_notify(job_struct_t* pjob_to_notify, 
                job_struct_t* pjob_to_run, 
                bool from_isr);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @param ticks_to_wait: timeout time, use "portMAX_DELAY" for blocking.
/// @returns received job to launch.
job_struct_t* __job_notify_take(TickType_t ticks_to_wait);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @returns: received job to launch.
/// @note calls job_notify_take() with portMAX_DELAY.
job_struct_t* __job_sleep_until_notified();

#endif