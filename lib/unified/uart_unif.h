#ifdef __cplusplus
extern "C" {
#endif

#ifndef _UART_UNIF_H_
#define _UART_UNIF_H_

#include "board_parser.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "job_driver.h"

#ifdef BUILD_FOR_STM32
#include "queue.h"
#endif

#ifdef UNIF_UART_WRITE_BUF_SIZE
#define __UNIF_UART_WRITE_BUF_SIZE UNIF_UART_WRITE_BUF_SIZE
#else
#define __UNIF_UART_WRITE_BUF_SIZE __MAX_JOB_ARGS_LEN_BYTE
#endif

#ifdef BUILD_FOR_STM32
#define UART_DATA           0
#define UART_FIFO_OVF       1
#define UART_BUFFER_FULL    2
#define UART_BREAK          3
#define UART_PARITY_ERR     4
#define UART_FRAME_ERR      5
// This struct mimics the ESP32 approach for uart events in simpler form.
typedef struct uart_event_t{
    uint8_t type;
    uint16_t size;
}uart_event_t;

void MX_USART2_UART_Init(void);

#endif

int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args);

int32_t uart_unif_write(const char *msg);

int32_t uart_unif_writef(const char *format, ...);

int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout);

// int32_t uart_unif_flush_inp(void);

int32_t uart_unif_flush(void);

int32_t uart_unif_deinit(void);

#endif

#ifdef __cplusplus
}
#endif