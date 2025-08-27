#include "delay_unif.h"
#include "board_parser.h"

#ifdef BUILD_FOR_STM32
#include "task.h"
#endif // BUILD_FOR_STM32

void jes_delay_job_ms(uint32_t ms){
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

uint32_t __get_systime_ms(void){
    return xTaskGetTickCount();
}
