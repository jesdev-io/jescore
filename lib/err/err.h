#ifndef _ERR_H_
#define _ERR_H_

/// @file Error types.

/// @brief Enums of errors.
typedef enum err{
    e_err_no_err,
    e_err_mem_null,
    e_err_is_zero,
    e_err_param,
    e_err_peripheral_block,
    e_err_core_fail,
    e_err_too_long,
    e_err_unknown_job,
}err_t;

#endif