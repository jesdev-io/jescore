/**
 * @file test_runner.c
 * @author jake-is-ESD-protected
 * @date 2025-06-29
 * @brief Main test runner file.
 *
 *
 */


#include "../common_test.h"
#include "board_parser.h"
#include "FreeRTOSConfig.h"
#include "task.h"

static TaskHandle_t pdispatch;

int main(void){
    xTaskCreate(test_all, "dispatch", 2048, NULL, 1, &pdispatch);
    vTaskStartScheduler();
    return 0;
}
