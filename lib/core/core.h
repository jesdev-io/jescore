#ifndef _CORE_H_
#define _CORE_H_

/// @file Main core acting as finite state machine.

#include "Arduino.h"
#include "err.h"
#include "commands.h"
#include "job_driver.h"


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
/// @param job_list (job_struct_t*): pointers to job functions in array format.
typedef struct core_t{
    state_t state;
    job_struct_t* job_list;
}core_t;


/// @brief Core initializer.
/// @return status, `e_no_err` if OK.
/// @note Registers/launches itself and CLI if `JES_DISABLE_CLI` is not defined
jes_err_t __core_init();


/// @brief Core error handler.
/// @param e: Error to handle.
/// @param args: optional additional arguments.
static void __core_err_handler_inline(jes_err_t e, void* args);


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
                    bool from_isr);


/// @brief Main core job. Handles calls and runs jobs.
/// @param p: Mandatory args pointer.
void __core_job(void* p);

#endif