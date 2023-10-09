#ifndef _ERR_H_
#define _ERR_H_

/*@file 
* @brief Error interpreter and handling */

/* @brief Types of errors
*/
typedef enum err{
    e_no_err,
    e_mem_null,
    e_is_zero,
    e_param,
    e_peripheral_block,
    e_core_fail,
    e_too_long,
}err_t;

/*@brief General error handler
* @param e (err_t): Error to handle
* @param args (void*): optional additional arguments 
*/
void err_handler(err_t e, void* args);

#endif