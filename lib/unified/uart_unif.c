#include "uart_unif.h"

QueueHandle_t* p_queue;

#if defined(BUILD_FOR_ESP32)
#include "driver/uart.h"
#define BASE_UART UART_NUM_0

int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args){
    uart_config_t uart_config = {
        .baud_rate = (int)baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    p_queue = (QueueHandle_t*)args;
    esp_err_t stat = uart_driver_install(BASE_UART, rx_buf_len, tx_buf_len, 20, p_queue, 0);
    if(stat != ESP_OK){
        return (int32_t)stat;
    }
    stat = uart_param_config(BASE_UART, &uart_config);
    if(stat != ESP_OK){
        return (int32_t)stat;
    }
    stat = uart_set_pin(BASE_UART, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if(stat != ESP_OK){
        return (int32_t)stat;
    }
    stat = uart_flush(BASE_UART);
    if(stat != ESP_OK){
        return (int32_t)stat;
    }
    return 0;
}

int32_t uart_unif_write(const char *msg) {
    uint16_t len = strlen(msg);
    return uart_write_bytes(BASE_UART, msg, len);
}

int32_t uart_unif_writef(const char *format, ...) {
    char buffer[__UNIF_UART_WRITE_BUF_SIZE];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len < 0) return -1;
    len = (len < sizeof(buffer)) ? len : sizeof(buffer) - 1;
    return uart_write_bytes(BASE_UART, buffer, len);
}

int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout){
    return uart_read_bytes(BASE_UART, buf, len, (TickType_t)timeout);
}

int32_t uart_unif_flush_inp(void){
    return uart_flush_input(BASE_UART);
}

int32_t uart_unif_flush(void){
    return uart_flush(BASE_UART);
}

int32_t uart_unif_deinit(void){
    /// TODO: 
    return 0;
}

#elif defined(__AVR__)

 int32_t uart_unif_write(char* msg, uint32_t len, uint32_t timeout){
    
}

 int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout){
    
}

#elif defined(BUILD_FOR_STM32)
#include "board_parser.h"
#include "projdefs.h"
#include "portmacro.h"

#define __JES_STM32_UART_BUF_LEN_BYTE (__MAX_JOB_NAME_LEN_BYTE + __MAX_JOB_ARGS_LEN_BYTE)

// shared buf for the RxCpltCallback and uart_unif_read
static uint8_t uart_buf[__JES_STM32_UART_BUF_LEN_BYTE]; 

 // the right huart has to be defined externally (depends on the board)
UART_HandleTypeDef huart_num;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t len){
    uart_event_t evt;
    evt.type = UART_DATA;
    evt.size = len;
    BaseType_t woke_task = pdFALSE;
    xQueueSendFromISR(*p_queue, (void*)&evt, &woke_task);
    // uart_unif_write("jescore $ ");

    HAL_UARTEx_ReceiveToIdle_IT(huart, uart_buf, __JES_STM32_UART_BUF_LEN_BYTE);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if(huart->Instance==USART2)
    {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK){
            /// TODO: figure out how to return driver issues for this
        }
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX */
        GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

void MX_USART2_UART_Init(void){
  huart_num.Instance = USART2;
  huart_num.Init.BaudRate = 115200;
  huart_num.Init.WordLength = UART_WORDLENGTH_8B;
  huart_num.Init.StopBits = UART_STOPBITS_1;
  huart_num.Init.Parity = UART_PARITY_NONE;
  huart_num.Init.Mode = UART_MODE_TX_RX;
  huart_num.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart_num.Init.OverSampling = UART_OVERSAMPLING_16;
  huart_num.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart_num.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart_num) != HAL_OK){
    /// TODO: figure out how to return driver issues for this
  }
}

void USART2_IRQHandler(void){
    HAL_UART_IRQHandler(&huart_num);
}

int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args){
    *(QueueHandle_t*)args = xQueueCreate(5, sizeof(uart_event_t));
    p_queue = (QueueHandle_t*)args;
    MX_USART2_UART_Init();
    HAL_StatusTypeDef stat = HAL_UARTEx_ReceiveToIdle_IT(&huart_num, uart_buf, __JES_STM32_UART_BUF_LEN_BYTE);
    if(stat != HAL_OK) return -1;
    return 0;
}

int32_t uart_unif_write(const char* msg){
    return HAL_UART_Transmit(&huart_num, (uint8_t*)msg, (uint16_t)strlen((char*)msg), HAL_MAX_DELAY);
}

int32_t uart_unif_writef(const char *format, ...) {
    return 0;
    char buffer[__JES_STM32_UART_BUF_LEN_BYTE];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len < 0) return -1;
    len = (len < sizeof(buffer)) ? len : sizeof(buffer) - 1;
    return HAL_UART_Transmit(&huart_num, (uint8_t*)buffer, len, HAL_MAX_DELAY);
}

int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout){
    UNUSED(timeout);
    if(len > __JES_STM32_UART_BUF_LEN_BYTE) return -1;
    for(uint32_t i = 0; i < len; i++){
        buf[i] = (char)uart_buf[i];
    }
    return 0;
}

int32_t uart_unif_flush(void){
    __HAL_UART_DISABLE(&huart_num);
    __HAL_UART_CLEAR_FLAG(&huart_num, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
    __HAL_UART_ENABLE(&huart_num);
    memset(uart_buf, 0, __JES_STM32_UART_BUF_LEN_BYTE);
    return 0;
}

int32_t uart_unif_deinit(void){
    /// TODO: 
    __USART2_CLK_DISABLE();
    return 0;
}

#else
    #if defined(ARDUINO) // Fallback to Arduino FW which might be able to cover the used board
    #include <Arduino.h>
     int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args){
        Serial.begin(baud);
        return 0;
    }

     int32_t uart_unif_write(const char *msg){
        return Serial.print(msg);
    }

     int32_t uart_unif_writef(const char *format, ...){
        char buffer[__UNIF_UART_WRITE_BUF_SIZE];
        va_list args;
        va_start(args, format);
        int len = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        if (len < 0) return -1;
        len = (len < sizeof(buffer)) ? len : sizeof(buffer) - 1;
        return Serial.print()
    }

     int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout){

    }

     int32_t uart_unif_flush_inp(void){

    }

     int32_t uart_unif_flush(void){

    }

    int32_t uart_unif_deinit(void){
        /// TODO: 
        return 0;
    }
    #else
    #error No valid processor architecture fur UART hardware!
    #endif

#endif