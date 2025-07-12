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

#endif

#ifdef __cplusplus
}
#endif