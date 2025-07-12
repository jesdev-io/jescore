#ifdef __cplusplus
extern "C" {
#endif

#ifndef _JES_ERR_H_
#define _JES_ERR_H_

/// @file Error types.

/// @brief Enums of errors.
typedef enum jes_err{
    e_err_no_err,
    e_err_mem_null,
    e_err_is_zero,
    e_err_param,
    e_err_peripheral_block,
    e_err_core_fail,
    e_err_duplicate,
    e_err_too_long,
    e_err_unknown_job,
    e_err_leading_whitespace,
    e_err_prohibited,
    e_err_driver_fail
}jes_err_t;

#endif

#ifdef __cplusplus
}
#endif