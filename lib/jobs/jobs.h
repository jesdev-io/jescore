#ifndef _JOBS_H_
#define _JOBS_H_

#include <Arduino.h>
#include "err.h"
#include "commands.h"

#define MAX_JOB_NAME_LEN_BYTE   16

/*@brief Abstraction struct for jobs to do
* @param name (char*): job name (callable by CLI)
* @param handle (TaskHandle_t): handle to task (FreeRTOS)
* @param mem_size (uint32_t): allocated stack size for task
* @param priority (uint8_t): priority of task
* @param function void(*)(void* p): function itself, function pointer
* @param args (void*): optional args obtained from CLI
* @param caller (origin_t): Requesting entity of job
* @param pn (job_struct_t*): pointer to next job (llist)*/
typedef struct job_struct_t{
    char name[MAX_JOB_NAME_LEN_BYTE] = {0};
    TaskHandle_t handle = NULL;
    uint32_t mem_size = 0;
    uint8_t priority = 0;
    void (*function) (void* p) = NULL;
    char args[MAX_JOB_NAME_LEN_BYTE] = {0};
    origin_t caller = e_origin_undefined;
    struct job_struct_t* pn = NULL;
}job_struct_t;


/*@brief Job struct setter and list appender
* @param job_list (job_struct_t**): head of job-llist
* @param n (char*): job name (callable by CLI)
* @param m (uint32_t): allocated stack size for task
* @param p (uint8_t): priority of job
* @param f (void (*)(void* p)): function itself, function pointer
* @returns status (err_t): `e_no_err` if OK
* @note Checks for plausible task parameters,
* e.g. forbids a mem size of 0 or empty
* task function pointers*/
err_t __register_job(job_struct_t** job_list,
                     const char* n, 
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p));


/*@brief Job getter based on name identifier
* @param job_list (job_struct_t**): head of job-llist
* @param n (char*): job name (callable by CLI)
* @returns job handle (job_struct_t*)*/
job_struct_t* __get_job(job_struct_t** job_list, char* n);


/*@brief Job creation wrapper 
* @param job_list (job_struct_t**): head of job-llist
* @param n (char*): job name (callable by CLI)
* @returns status (err_t), `e_no_err` if OK
* @note Checks if task memory could be allocated.*/
err_t __launch_job(job_struct_t** job_list, char* n);


/*@brief Job creation wrapper for cli based commands
* @param job_list (job_struct_t**): head of job-llist
* @param n (char*): job name (callable by CLI)
* @param args (char*): optional args obtained from CLI
* @returns status (err_t), `e_no_err` if OK
* @note Checks if task memory could be allocated. Task handle
* is passed along to task itself for read-only purposes*/
err_t __launch_job_from_cli(job_struct_t** job_list, char* n, char* args);

/*@brief Helper function for safety assertments around strcpy()
* @param buf (char*): name buffer (job_struct->name)
* @param name (char*): given name to copy (comes from CLI)
* @returns status (err_t), `e_no_err` if OK*/
static err_t __copy_name(char* buf, char* n);


/*@brief Task notification wrapper for FreeRTOS "xTaskNotify()"
* @param pjob_to_notify (task_struct_t*), handle to job to be notified
* @param pjob_to_run (task_struct_t*), job that should be launched by the notified job
* @param from_isr bool, specifies ISR or non-ISR origin*/
void job_notify(job_struct_t* pjob_to_notify, 
                job_struct_t* pjob_to_run, 
                bool from_isr);


/*@brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake"
* @param ticks_to_wait TickType_t, timeout time, use "portMAX_DELAY" for blocking
* @returns job_struct_t*, received job to launch*/
job_struct_t* job_notify_take(TickType_t ticks_to_wait);


/*@brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake"
* @returns job_struct_t*, received job to launch
* @note calls job_notify_take() with portMAX_DELAY*/
job_struct_t* sleep_until_notified();

#endif