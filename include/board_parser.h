/**
 * @file board_parser.h
 * @author jake-is-ESD-protected
 * @date 2025-07-12
 * @brief Board parser for avstraction of different hardware.
 *
 * This file defines the hardware specific inclusions and macros
 * needed to make jescore be able to interact with the underlying hardware.
 * While the ESP-platform is quite well abstracted already and does not need
 * additional configuration for different boards, the STM platform requires 
 * imports per family and macros per board. In order to run jescore with the
 * CLI enabled, an STM-based project needs these macros for the specific board:
 * 
    #define BUILD_PLATFORM_NAME "<board type/name>"
    #define USART_RCC_PERIPH RCC_PERIPHCLK_USART<num>
    #define #define USART_CLK_SRC_DEFAULT(PeriphClkInit_struct) PeriphClkInit_struct.Usart<num>ClockSelection = __HAL_RCC_GET_USART<num>_SOURCE()
    #define USART_CLK_ENABLE() __HAL_RCC_USART<num>_CLK_ENABLE()
    #define USART_CLK_GPIO_ENABLE() __HAL_RCC_GPIO<port>_CLK_ENABLE()
    #define USART_CLK_GPIO_DISABLE() __USART<num>_CLK_DISABLE();
    #define USART_NUM USART<num>
    #define USART_GPIO_TX_PORT GPIO<port>
    #define USART_GPIO_RX_PORT GPIO<port>
    #define USART_GPIO_TX_NUM GPIO_PIN_<pin>
    #define USART_GPIO_RX_NUM GPIO_PIN_<pin>
    #define USART_GPIO_TX_ALT GPIO_AF<AF num>_USART<num>
    #define USART_GPIO_RX_ALT GPIO_AF<AF num>_USART<num>
    #define USART_IRQn_NUM USART<num>_IRQn
 * 
 * where <num> is the UART peripheral index (often index 2), <port> is the GPIO
 * bank of the UART TX and RX <pin>s. <AF num> denotes the alternate function
 * index and is specific to the MCU. If BUILD_PLATFORM_NAME is not set, it defaults
 * to "STM32" without the specific family number.
 */

#ifndef _BOARD_PARSER_H_
#define _BOARD_PARSER_H_

// --- ESP32 -- //
#ifdef ESP32
#define BUILD_FOR_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/uart.h"

#define BOARD_MIN_JOB_HEAP_MEM 2048
#define BUILD_PLATFORM_NAME "ESP32"

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

#ifdef STM32G431xx
#include "stm32g4xx_hal.h"
#define BUILD_PLATFORM_NAME "STM32G431"
#define USART_RCC_PERIPH RCC_PERIPHCLK_USART2
#define USART_CLK_SRC_DEFAULT(PeriphClkInit_struct) PeriphClkInit_struct.Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE()
#define USART_CLK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()
#define USART_CLK_GPIO_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_CLK_GPIO_DISABLE() __USART2_CLK_DISABLE();
#define USART_NUM USART2
#define USART_GPIO_TX_PORT GPIOA
#define USART_GPIO_RX_PORT GPIOA
#define USART_GPIO_TX_NUM GPIO_PIN_2
#define USART_GPIO_RX_NUM GPIO_PIN_3
#define USART_GPIO_TX_ALT GPIO_AF7_USART2
#define USART_GPIO_RX_ALT GPIO_AF7_USART2
#define USART_IRQn_NUM USART2_IRQn
#define MAX_JESCORE_KB_MEM 28 // default in FreeRTOSConfig.h is too much

#endif // STM32G431xx

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

#ifdef STM32H753xx
#include "stm32h7xx_hal.h"
#define BUILD_PLATFORM_NAME "STM32H753"
#define USART_RCC_PERIPH RCC_PERIPHCLK_USART3
#define USART_CLK_SRC_DEFAULT(PeriphClkInit_struct) PeriphClkInit_struct.Usart234578ClockSelection = __HAL_RCC_GET_USART234578_SOURCE()
#define USART_CLK_ENABLE() __HAL_RCC_USART3_CLK_ENABLE()
#define USART_CLK_GPIO_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART_CLK_GPIO_DISABLE() __USART3_CLK_DISABLE()
#define USART_NUM USART3
#define USART_GPIO_TX_PORT GPIOD
#define USART_GPIO_RX_PORT GPIOD
#define USART_GPIO_TX_NUM GPIO_PIN_8
#define USART_GPIO_RX_NUM GPIO_PIN_9
#define USART_GPIO_TX_ALT GPIO_AF7_USART3
#define USART_GPIO_RX_ALT GPIO_AF7_USART3
#define USART_IRQn_NUM USART3_IRQn
#endif // STM32H753xx

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

#ifdef STM32L476xx
#define BUILD_PLATFORM_NAME "STM32L476"
#define USART_RCC_PERIPH RCC_PERIPHCLK_USART2
#define USART_CLK_SRC_DEFAULT(PeriphClkInit_struct) PeriphClkInit_struct.Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE()
#define USART_CLK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()
#define USART_CLK_GPIO_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_CLK_GPIO_DISABLE() __USART2_CLK_DISABLE();
#define USART_NUM USART2
#define USART_GPIO_TX_PORT GPIOA
#define USART_GPIO_RX_PORT GPIOA
#define USART_GPIO_TX_NUM GPIO_PIN_2
#define USART_GPIO_RX_NUM GPIO_PIN_3
#define USART_GPIO_TX_ALT GPIO_AF7_USART2
#define USART_GPIO_RX_ALT GPIO_AF7_USART2
#define USART_IRQn_NUM USART2_IRQn
#endif // STM32L476xx

#ifdef STM32L432xx
#define BUILD_PLATFORM_NAME "STM32L432"
#define USART_RCC_PERIPH RCC_PERIPHCLK_USART2
#define USART_CLK_SRC_DEFAULT(PeriphClkInit_struct) PeriphClkInit_struct.Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE()
#define USART_CLK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()
#define USART_CLK_GPIO_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_CLK_GPIO_DISABLE() __USART2_CLK_DISABLE();
#define USART_NUM USART2
#define USART_GPIO_TX_PORT GPIOA
#define USART_GPIO_RX_PORT GPIOA
#define USART_GPIO_TX_NUM GPIO_PIN_2
#define USART_GPIO_RX_NUM GPIO_PIN_15
#define USART_GPIO_TX_ALT GPIO_AF7_USART2
#define USART_GPIO_RX_ALT GPIO_AF3_USART2
#define USART_IRQn_NUM USART2_IRQn
#endif // STM32L432xx

#endif // STM32L4

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

#define BOARD_MIN_JOB_HEAP_MEM 512
#ifndef BUILD_PLATFORM_NAME
#define BUILD_PLATFORM_NAME "STM32"
#endif

#endif

#endif

