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

/// @brief Print the roundtrip time of a job.
/// @param p Pointer to own job struct.
/// @note  Call `bench <job_name>` to get the roundtrip time
///        of the job with the name <job_name>.
/// @note  The bench command does not execute jobs, it just
///        fetches the logged start and finish times of a
///        job. If the selected job has not been run before,
///        an error will be displayed. Jobs set up as loops need
///        their own time stamp hooks in place, see
///        `__job_set_timing_begin()`.
void __base_job_bench(void* p);

#endif

#ifdef __cplusplus
}
#endif