/**
 * @file test_runner.cpp
 * @author jake-is-ESD-protected
 * @date 2025-06-29
 * @brief Dual C/C++ compatible test runner.
 *
 * Works as both .cpp for Arduino and .c for STM32
 */

#include "board_parser.h"

#ifdef BUILD_FOR_STM32
/* -----------------------------------------------------------------
BEGIN: STM32 platform test runner (C-compatible)
----------------------------------------------------------------- */
#include "common_test.h"
#include "jescore_api.h"
#include "board_parser.h"
#include "FreeRTOSConfig.h"
#include "task.h"

static TaskHandle_t pdispatch;

int main(void) {
    xTaskCreate(test_all, "dispatch", 2048, NULL, 1, &pdispatch);
    jes_dispatch();
    return 0;
}
/* -----------------------------------------------------------------
END: STM32 platform test runner
----------------------------------------------------------------- */

#elif defined(BUILD_FOR_ESP32)
/* -----------------------------------------------------------------
BEGIN: ESP32 platform (C++ Arduino-compatible)
----------------------------------------------------------------- */
#include "common_test.h"

void setup() {
    test_all(NULL);  // nullptr instead of NULL for C++
}

void loop() {
    // Empty
}
/* -----------------------------------------------------------------
END: ESP32 platform
----------------------------------------------------------------- */

#else
#error "UNSPECIFIED BUILD TYPE"
#endif