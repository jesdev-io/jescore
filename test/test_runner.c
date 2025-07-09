/**
 * @file test_runner.c
 * @author jake-is-ESD-protected
 * @date 2025-06-29
 * @brief Main test runner file.
 *
 *
 */

#include "board_parser.h"

#ifdef BUILD_FOR_STM32
/* -----------------------------------------------------------------
BEGIN: STM32 platform test runner.
> Starts from the "true" main function and needs a FreeRTOS
> dispatcher, since main() then never escapes vTaskStartScheduler().
----------------------------------------------------------------- */

#include "common_test.h"
#include "board_parser.h"
#include "FreeRTOSConfig.h"
#include "task.h"

static TaskHandle_t pdispatch;

int main(void){
    xTaskCreate(test_all, "dispatch", 2048, NULL, 1, &pdispatch);
    vTaskStartScheduler();
    return 0;
}
/* -----------------------------------------------------------------
END: STM32 platform test runner.
----------------------------------------------------------------- */

#elif BUILD_FOR_ESP32
/* -----------------------------------------------------------------
BEGIN: ESP32 platform + Arduino test runner.
> Already dispatched.
----------------------------------------------------------------- */

#include "common_test.h"

void setup(){
    test_all(__null);
}

void loop(){

}
/* -----------------------------------------------------------------
END: ESP32 platform + Arduino test runner.
----------------------------------------------------------------- */

#else

#error "UNSPECIFIED BUILD TYPE"

#endif