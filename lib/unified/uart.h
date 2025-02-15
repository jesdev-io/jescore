#pragma once

#include <inttypes.h>

#if defined(ESP32)
#include "driver/uart.h"
#define BASE_UART UART_NUM_0

inline int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args){
    uart_config_t uart_config = {
        .baud_rate = (int)baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    QueueHandle_t* p_queue = (QueueHandle_t*)args;
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

inline int32_t uart_unif_write(uint8_t* msg, uint32_t len){
    return uart_write_bytes(BASE_UART, (const char*) msg, len);
}

inline int32_t uart_unif_read(uint8_t* buf, uint32_t len, uint32_t timeout){
    return uart_read_bytes(BASE_UART, buf, len, (TickType_t)timeout);
}

inline int32_t uart_unif_flush_inp(void){
    return uart_flush_input(BASE_UART);
}

inline int32_t uart_unif_flush(void){
    return uart_flush(BASE_UART);
}

#elif defined(__AVR__)

inline int32_t uart_unif_write(uint8_t* msg, uint32_t len, uint32_t timeout){
    
}

inline int32_t uart_unif_read(uint8_t* buf, uint32_t len, uint32_t timeout){
    
}

#elif defined(STM32)

inline int32_t uart_unif_write(uint8_t* msg, uint32_t len, uint32_t timeout){
    
}

inline int32_t uart_unif_read(uint8_t* buf, uint32_t len, uint32_t timeout){
    
}

#else
#error No valid processor architecture fur UART hardware!
#endif