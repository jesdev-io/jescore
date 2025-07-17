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

#endif

/// @brief Unified UART initializer.
/// @param baud Baudrate.
/// @param rx_buf_len RX buffer length in byte.
/// @param tx_buf_len TX buffer length in byte.
/// @param args Optional args.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
/// @note What arguments are used how is determined by the platform, see uart_unif.c
int32_t uart_unif_init(uint32_t baud, uint32_t rx_buf_len, uint32_t tx_buf_len, void* args);

/// @brief Write a string to the platform's UART.
/// @param msg String to send.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
/// @note Calls strlen() internally.
int32_t uart_unif_write(const char *msg);

/// @brief Write a formated string to the platform's UART.
/// @param format String buffer.
/// @param ... Format arguments.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
int32_t uart_unif_writef(const char *format, ...);

/// @brief Read a string from the platform's UART.
/// @param buf Buffer to read into.
/// @param len Length of data to read.
/// @param timeout Point at which to return from this call if data takes long to arrive.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
/// @note What arguments are used how is determined by the platform, see uart_unif.c
int32_t uart_unif_read(char* buf, uint32_t len, uint32_t timeout);

/// @brief Flush the platform's UART.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
int32_t uart_unif_flush(void);

/// @brief Unified UART de-initializer.
/// @return Status. 0 if OK, -1 in platform specific driver failure.
int32_t uart_unif_deinit(void);

#endif

#ifdef __cplusplus
}
#endif