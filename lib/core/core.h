#ifndef _CORE_H_
#define _CORE_H_

/*@brief Main core acting as finite state machine */

#include "Arduino.h"
#include "err.h"
#include "commands.h"
#include "jobs.h"

#define CORE_JOB_NAME       "core"
#define INIT_CLI_JOB_NAME   "initcli"


/// @brief Types of states.
/// @note Always prefixed with "e_state".
typedef enum state_t{
    e_state_init,
    e_state_idle,
    e_state_fault,
    e_state_spawning,
    NUM_STATES
} state_t;


/// @brief Main core object, has one instance.
/// @param state (state_t): current state of core (fsm).
/// @param task_list (task_struct_t*): pointers to task functions in array format.
typedef struct core_t{
    state_t state;
    job_struct_t* job_list;
}core_t;


/// @brief Core initializer.
/// @return returns status, `e_no_err` if OK.
/// @note Registers/launches itself and CLI if `JES_DISABLE_CLI` is not defined
err_t __core_init();


/// @brief Wrapper for __register_job(), register a new job to do.
/// @param n: job name (callable by CLI).
/// @param m: allocated stack size for task.
/// @param p: priority of job.
/// @param f: function itself, function pointer.
/// @return status, `e_no_err` if OK.
err_t __core_register_job(const char* n, 
                        uint32_t m,
                        uint8_t p, 
                        void (*f)(void* p));


/// @brief Job getter based on name identifier.
/// @param n: job name (callable by CLI).
/// @return handle to job.
job_struct_t* __core_get_job(const char* n);


/// @brief Job creation wrapper.
/// @param n: job name (callable by CLI).
/// @return status, `e_no_err` if OK.
err_t __core_launch_job(const char* n);


/// @brief Main core job. Handles calls and runs jobs.
/// @param p: Mandatory args pointer.
void __core_job(void* p);

#endif