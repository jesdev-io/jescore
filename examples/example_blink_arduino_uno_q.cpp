#include <Arduino.h>
#include <jescore.h>
#include "stm32u5xx_hal.h"

// void SystemClock_Config(void);
// void error_handler();

// definitions for the STM32u585 on the Arduino Uno Q
// #define LED_PORT GPIOH
// #define LED_GPIO GPIO_PIN_10
// #define LED_ENABLE_PORT() __HAL_RCC_GPIOH_CLK_ENABLE()

void blink(void* p){
    static uint8_t act = 0;
    act = !act;
    while(act){
        HAL_GPIO_TogglePin(LED_PORT, LED_GPIO);
        jes_delay_job_ms(1000);
    }
}

void setup() {
    // HAL_Init();
    // SystemClock_Config();
    pinMode(PH10, OUTPUT);

    jes_init();
    jes_register_and_launch_job("blink", 256, 1, blink, 1);
    jes_dispatch();
}

void loop() {

}

// void SystemClock_Config(void) {
//     RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//     if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE4) != HAL_OK){
//         error_handler();
//     }
//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//     RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//     RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
//     RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
//     RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//     if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
//         error_handler();
//     }
//     RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
//                                 |RCC_CLOCKTYPE_PCLK3;
//     RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//     RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//     RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//     RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
//     if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK){
//         error_handler();
//     }
// }

// void error_handler(void){
//   __disable_irq();
//   while (1) { }
// }
