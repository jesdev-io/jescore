#ifdef __cplusplus
extern "C" {
#endif

#ifndef _DELAY_UNIF_H_
#define _DELAY_UNIF_H_

#include <inttypes.h>

/// @brief Delay a job in milliseconds.
/// @param ms Milliseconds of delay.
/// @note Timing is handled by FreeRTOS
void jes_delay_job_ms(uint32_t ms);

/// @brief Get the current system time since boot in milliseconds.
/// @return System time in milliseconds.
uint32_t __get_systime_ms(void);

#endif

#ifdef __cplusplus
}
#endif