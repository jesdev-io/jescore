#ifndef _JESCORE_API_H_
#define _JESCORE_API_H_

/// @file Top API layer for user access.

#include "core.h"
#include "jes_err.h"
#include "job_driver.h"

/// @brief Start the core and all of its abilities.
/// @return Status. Returns `e_no_err` in case of successful launch.
jes_err_t jes_init();


/// @brief Add a job (function block) to the list of all known jobs.
/// @param name: Name of job. Can't be longer than `MAX_JOB_NAME_LEN_BYTE`.
/// @param mem_size: Dynamic memory size for job.
/// @param priority: Priority of the job (1 is highest).
/// @param function: Function to run when the job is called. has to be of signature `void my_func(void* p)`.
/// @param is_loop: flag which describes the lifetime of the job.
/// @return Status. Returns `e_no_err` in case of successful registration.
jes_err_t register_job(const char* name,
                       uint32_t mem_size,
                       uint8_t priority,
                       void (*function)(void* p),
                       bool is_loop);


/// @brief Start a registered job.
/// @param name: String name of job as set in `register_job()` @
/// @return Status. Returns `e_no_err` in case of successful launch.
jes_err_t launch_job(const char* name);


/// @brief Add a job (function block) to the list of all known jobs.
/// @param name: Name of job. Can't be longer than `MAX_JOB_NAME_LEN_BYTE`.
/// @param mem_size: Dynamic memory size for job.
/// @param priority: Priority of the job (1 is highest).
/// @param function: Function to run when the job is called. has to be of signature `void my_func(void* p)`.
/// @param is_loop: flag which describes the lifetime of the job.
/// @return Status. Returns `e_no_err` in case of successful launch.
jes_err_t register_and_launch_job(const char* name,
                                  uint32_t mem_size,
                                  uint8_t priority,
                                  void (*function)(void* p),
                                  bool is_loop);


/// @brief Send string to printing job.
/// @param s: string to print.
/// @return Status. Returns `e_no_err` in case of successful launch.
jes_err_t jesprint(const char* s);


/// @brief Set the field `args` of the job.
/// @param s: String to insert into `args` field.
/// @return status, `e_no_err` if OK.
jes_err_t set_args(char* s);


/// @brief Get the field `args` of the job.
/// @return Pointer to `args` field of the job.
/// @attention  This function returns a pointer to the args buffer of the
///             job struct. Since this function is intended to be called from
///             within a job, the memory of the job struct persists, which 
///             makes a copy of the arg string redundant. If however something
///             goes wrong, this function will return NULL.
char* get_args(void);


/// @brief Set the field `param` of the job.
/// @param p: Arbitrary reference to parameter.
/// @return status, `e_no_err` if OK.
jes_err_t set_param(void* p);


/// @brief Get the field `param` of the job.
/// @return Pointer to `param` field of the job.
/// @attention Will return NULL on error.
void* get_param(void);


/// @brief Notify a job with an optional message.
/// @param name Name of job which should be notified.
/// @param notification Optional pointer to notification value.
void notify_job(const char* name, void* notification);


/// @brief Notify a job with an optional message from within an interrupt.
/// @param name Name of job which should be notified.
/// @param notification Optional pointer to notification value. 
void notify_job_ISR(const char* name, void* notification);


/// @brief Pause the calling job until a notification arrives.
/// @return The optional notification value.
void* wait_for_notification(void);
#endif