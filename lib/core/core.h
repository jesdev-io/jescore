#ifndef _CORE_H_
#define _CORE_H_

/*@brief Main core acting as finite state machine */

#include "Arduino.h"
#include "err.h"
#include "commands.h"
#include "jobs.h"


/*@brief Types of states 
* always prefixed with "e_state"*/
typedef enum state_t{
    e_state_init,
    e_state_idle,
    e_state_fault,
    e_state_spawning,
    NUM_STATES
} state_t;


/*@brief Main core object, has one instance
* @param state (state_t): current state of core (fsm)
* @param task_list (task_struct_t*): pointers to task functions in array format*/
typedef struct core_t{
    state_t state;
    job_struct_t* job_list;
}core_t;


/*@brief Core initializer 
* @returns status (err_t), `e_no_err` if OK
* @note Creates all tasks in default mode (stopped)*/
err_t core_init();


/*@brief Wrapper for __register_job(), register a new job to do
* @param n (char*): job name (callable by CLI)
* @param m (uint32_t): allocated stack size for task
* @param p (uint8_t): priority of job
* @param f (void (*)(void* p)): function itself, function pointer
* @returns status (err_t), `e_no_err` if OK
* @note Creates all tasks in default mode (stopped)*/
err_t core_register_job(const char* n, 
                        uint32_t m,
                        uint8_t p, 
                        void (*f)(void* p));


/*@brief Job getter based on name identifier
* @param n (char*): job name (callable by CLI)
* @returns job_struct_t* handle to job*/
job_struct_t* core_get_job(char* n);


/*@brief Job creation wrapper
* @param n (char*): job name (callable by CLI)
* @returns status (err_t), `e_no_err` if OK*/
err_t core_launch_job(char* n);


/*@brief Job creation wrapper for cli based commands
* @param n (char*): job name (callable by CLI)
* @param args (char*): optional args obtained from CLI
* @returns status (err_t), `e_no_err` if OK*/
err_t core_launch_job_from_cli(char* n, char* args);


void core_job(void* p);

#endif