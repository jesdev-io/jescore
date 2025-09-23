#ifdef __cplusplus
extern "C" {
#endif

#ifndef _JOB_DRIVER_H_
#define _JOB_DRIVER_H_

/// @file Driver for jobs. Handles job registration, launching and inter-job communication.

#include <stdint.h>
#include <string.h>
#include "board_parser.h"
#include "jes_err.h"
#include "commands.h"

#ifdef BUILD_FOR_STM32
#include "queue.h"
#endif

#define __GET_SAFE_SIZE(SIZE, LIMIT) ((SIZE)<=(LIMIT)?(SIZE):(LIMIT))
#ifndef MAX_JOB_NAME_LEN_BYTE
#define __MAX_JOB_NAME_LEN_BYTE     32
#else
#define __MAX_JOB_NAME_LEN_LIMIT    64
#define __MAX_JOB_NAME_LEN_BYTE __GET_SAFE_SIZE(MAX_JOB_NAME_LEN_BYTE, MAX_JOB_NAME_LEN_LIMIT)
#endif

#ifndef MAX_JOB_ARGS_LEN_BYTE
#define __MAX_JOB_ARGS_LEN_BYTE     32
#else
#define __MAX_JOB_ARGS_LEN_LIMIT    64
#define __MAX_JOB_ARGS_LEN_BYTE __GET_SAFE_SIZE(MAX_JOB_ARGS_LEN_BYTE, MAX_JOB_ARGS_LEN_LIMIT)
#endif

#define MAX_JOB_NOTIF_QUEUE_SIZE    4

typedef enum{
    e_role_core,
    e_role_base,
    e_role_user
}e_role_t;

/// @brief Abstraction struct for jobs to do.
/// @param name (char*): job name (callable by CLI).
/// @param handle (TaskHandle_t): handle to task (FreeRTOS).
/// @param mem_size (uint32_t): allocated stack size for task.
/// @param priority (uint8_t): priority of task.
/// @param function void(*)(void* p): function itself, function pointer.
/// @param args (char*): optional args obtained from CLI.
/// @param is_loop (uint8_t): defines if job runs forever or terminates
/// @param instances (uint8_t): number of active instances of same job type.
/// @param role (e_role_t): Role of job in usage context.
/// @param caller (origin_t): Requesting entity of job.
/// @param param (void*): Optional information.
/// @param error (jes_err_t): Error associated with fault from/in job. Gets used by the error handler
/// @param notif_queue (QueueHandle_t): Job's notification queue handle.
/// @param pn (job_struct_t*): Pointer to next job (llist).
typedef struct job_struct_t{
    char name[__MAX_JOB_NAME_LEN_BYTE];
    TaskHandle_t handle;
    uint32_t mem_size;
    uint8_t priority;
    void (*function) (void* p);
    char args[__MAX_JOB_ARGS_LEN_BYTE];
    uint8_t is_loop;
    uint8_t instances;
    e_role_t role;
    origin_t caller;
    void* param;
    jes_err_t error;
    QueueHandle_t notif_queue;
    uint32_t timing_begin;
    uint32_t timing_end;
    struct job_struct_t* pn;
}job_struct_t;


/// @brief Job struct setter and list appender.
/// @param n: job name (callable by CLI).
/// @param m: allocated stack size for task.
/// @param p: priority of job.
/// @param f: function itself, function pointer.
/// @param is_loop: flag which describes the lifetime of the job.
/// @param role: role of job in usage context. See `e_role_t` enum.
/// @returns status, `e_err_no_err` if OK.
/// @note Checks for plausible task parameters,
/// e.g. forbids a mem size of 0 or empty
/// task function pointers.
jes_err_t __job_register_job(const char* n, 
                         uint32_t m,
                         uint8_t p, 
                         void (*f)(void* p),
                         uint8_t is_loop,
                         e_role_t role);


/// @brief Job getter based on name identifier.
/// @param n: job name (callable by CLI).
/// @returns job handle.
job_struct_t* __job_get_job_by_name(const char* n);


/// @brief Job getter based on function signature.
/// @param f: function pointer of existing job.
/// @returns job handle.
job_struct_t* __job_get_job_by_func(void (*f)(void* p));


/// @brief Job getter based on task handle.
/// @param t: handle to task within job (FreeRTOS).
/// @returns job handle.
/// @attention  This will only work if the targeted job
///             is running, otherwise its handle will
///             be set to NULL.
job_struct_t* __job_get_job_by_handle(TaskHandle_t t);


/// @brief Job creation wrapper.
/// @param pj: Pointer to job to do.
/// @param o: Origin of activation.
/// @returns status, `e_err_no_err` if OK.
jes_err_t __job_launch_job(job_struct_t* pj, origin_t o);

/// @brief Job creation wrapper.
/// @param n: job name (callable by CLI).
/// @param o: Origin of activation.
/// @returns status, `e_err_no_err` if OK.
/// @note Checks if task memory could be allocated.
jes_err_t __job_launch_job_by_name(const char* n, origin_t o);


/// @brief Job creation wrapper.
/// @param n: job name (callable by CLI).
/// @param o: Origin of activation.
/// @param args: String of args (whitespace delimited string).
/// @returns status, `e_err_no_err` if OK.
/// @note Checks if task memory could be allocated.
/// @note Arguments are expected to be whitespace delimited substrings
///       in a string. They should not exceed `MAX_JOB_ARGS_LEN_BYTE`.
///       A copy is stored in the `args` field of the job.
jes_err_t __job_launch_job_by_name_args(const char* n, origin_t o, const char* args);


/// @brief Jescore runtime environment. This is the FreeRTOS task that
///        then calls the registered job inside its scope.
/// @param p Typeless pointer to job struct that should be run.
void __job_runtime_env(void* p);


/// @brief Helper function for safety assertments around strcpy().
/// @param buf: empty string buffer.
/// @param str: given string to copy.
/// @param max_len: max length of string to be copied.
/// @returns status, `e_err_no_err` if OK.
jes_err_t __job_copy_str(char* buf, char* str, uint16_t max_len);


/// @brief Task notification wrapper for FreeRTOS "xTaskNotify()".
/// @param pjob_to_notify: handle to job to be notified.
/// @param notif: Arbitrary 32 bit notification.
/// @param from_isr: specifies ISR or non-ISR origin.
void __job_notify_generic(job_struct_t* pjob_to_notify, 
                          void* notif,
                          uint8_t from_isr);


/// @brief Task notification wrapper for FreeRTOS "xTaskNotify()".
/// @param pjob_to_notify: handle to job to be notified.
/// @param pjob_to_run: job that should be launched by the notified job.
/// @param from_isr: specifies ISR or non-ISR origin.
void __job_notify_with_job(job_struct_t* pjob_to_notify, 
                           job_struct_t* pjob_to_run, 
                           uint8_t from_isr);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @param ticks_to_wait: timeout time, use "portMAX_DELAY" for blocking.
/// @returns Arbitrary 32 bit notification sent in `__job_notify_generic()`
void* __job_notify_generic_take(TickType_t ticks_to_wait);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @param ticks_to_wait: timeout time, use "portMAX_DELAY" for blocking.
/// @returns received job to launch.
job_struct_t* __job_notify_with_job_take(TickType_t ticks_to_wait);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @returns: Arbitrary 32 bit notification sent in `__job_notify_generic()`
/// @note calls job_notify_take() with portMAX_DELAY.
void* __job_sleep_until_notified_generic(void);


/// @brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake".
/// @returns: received job to launch.
/// @note calls job_notify_take() with portMAX_DELAY.
job_struct_t* __job_sleep_until_notified_with_job(void);


/// @brief Set the field `args` of the job.
/// @param s: String to insert into `args` field.
/// @param pj: Pointer to job.
/// @return status, `e_err_no_err` if OK.
jes_err_t __job_set_args(char* s, job_struct_t* pj);


/// @brief Get the field `args` of the job.
/// @param pj: Pointer to job. 
/// @return Pointer to `args` field of the job.
/// @attention  This function returns a pointer to the args buffer of the
///             job struct. Since this function is intended to be called from
///             within a job, the memory of the job struct persists, which 
///             makes a copy of the arg string redundant. If however something
///             goes wrong, this function will return NULL.
char* __job_get_args(job_struct_t* pj);


/// @brief Set the field `param` of the job.
/// @param p: Arbitrary reference to parameter.
/// @param pj: Pointer to job. 
/// @return status, `e_err_no_err` if OK.
jes_err_t __job_set_param(void* p, job_struct_t* pj);


/// @brief Get the field `param` of the job.
/// @param pj: Pointer to job. 
/// @return Pointer to `param` field of the job.
/// @attention Will return NULL on error.
void* __job_get_param(job_struct_t* pj);

/// @brief Set the field `timing_begin` of the job.
/// @param t Current time.
/// @param pj Pointer to job.
void __job_set_timing_begin(uint32_t t, job_struct_t* pj);

/// @brief Set the field `timing_end` of the job.
/// @param t Current time.
/// @param pj Pointer to job.
void __job_set_timing_end(uint32_t t, job_struct_t* pj);

/// @brief Get the difference of `timing_end` and `timing_begin` of the job.
/// @param pj Pointer to job.
uint32_t __job_get_timing(job_struct_t* pj);

#endif

#ifdef __cplusplus
}
#endif