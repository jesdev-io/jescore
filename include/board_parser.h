#ifndef _BOARD_PARSER_H_
#define _BOARD_PARSER_H_

// --- ESP32 -- //
#ifdef ESP32
#define BUILD_FOR_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#endif

// --- STM32 --- //
#ifdef STM32C0
#include "stm32c0xx.h"
#include "system_stm32c0xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F0
#include "stm32f0xx.h"
#include "system_stm32f0xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F1
#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F2
#include "stm32f2xx.h"
#include "system_stm32f2xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F3
#include "stm32f3xx.h"
#include "system_stm32f3xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F4
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32F7
#include "stm32f7xx.h"
#include "system_stm32f7xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32G0
#include "stm32g0xx.h"
#include "system_stm32g0xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32G4
#include "stm32g431xx.h"
#include "system_stm32g4xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32H5
#include "stm32h5xx.h"
#include "system_stm32h5xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32H7
#include "stm32h7xx.h"
#include "system_stm32h7xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32L0
#include "stm32l0xx.h"
#include "system_stm32l0xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32L1
#include "stm32l1xx.h"
#include "system_stm32l1xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32L4
#include "stm32l4xx.h"
#include "system_stm32l4xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32L5
#include "stm32l5xx.h"
#include "system_stm32l5xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32U0
#include "stm32u0xx.h"
#include "system_stm32u0xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32U5
#include "stm32u5xx.h"
#include "system_stm32u5xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32WBA
#include "stm32wbaxx.h"
#include "system_stm32wbaxx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32WB
#include "stm32wbxx.h"
#include "system_stm32wbxx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32WL
#include "stm32wlxx.h"
#include "system_stm32wlxx.h"
#define BUILD_FOR_STM32
#endif

#ifdef STM32N6
#include "stm32n6xx.h"
#include "system_stm32n6xx.h"
#define BUILD_FOR_STM32
#endif

#ifdef BUILD_FOR_STM32
#include "FreeRTOS.h"
#endif

#endif

