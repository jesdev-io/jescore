#ifdef __cplusplus
extern "C" {
#endif

#ifndef _BASE_JOBS_H_
#define _BASE_JOBS_H_

#include "commands.h"
#include "jes_err.h"

/// @brief Write an incoming string back to UART. 
/// @param p Pointer to own job struct.
/// @note Call this on the CLI with `jescore echo <string>`
void __base_job_echo(void* p);

/// @brief Print an overview of CLI callable jobs.
/// @param p Pointer to own job struct.
/// @note Call this on the CLI with `jescore help`
void __base_job_help(void* p);

/// @brief Print an overview of registered and running jobs.
/// @param p Pointer to own job struct.
/// @note When called with the arg `-a`, it also prints the
///       base jobs and information about the core. The flag
///       `-aa` will print all jobs, even th ones not accessible
///       to the user program.
/// @note Call this on the CLI with `jescore stats [-a, -aa]`
void __base_job_stats(void* p);

#endif

#ifdef __cplusplus
}
#endif