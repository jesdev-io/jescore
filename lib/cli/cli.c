#include "string.h"
#include "jes_err.h"
#include "cli.h"
#include "job_driver.h"
#include "base_jobs.h"
#include "job_names.h"
#include "uart_unif.h"
#include "core.h"


static QueueHandle_t queue_uart;
static uint8_t is_init = 0;
SemaphoreHandle_t cli_lock;

/// @brief Reprint the CLI header.
/// @returns status, `e_err_no_err` if OK.
/// @note This is neccessary to determine if a transaction has finished on the 
///       client side.
static jes_err_t __cli_reprint_header(void){
    if(uart_unif_write(CLI_HEADER) != 0) return e_err_driver_fail;
    return e_err_no_err;
}

/// @brief Get the index of the first white space in a string.
/// @param buf String to analyze.
/// @param len Length of string.
/// @return Index of white space in string. Returns -1 for the absence of a white space.
static int16_t __get_ws_index(char* buf, uint16_t len){
    int16_t i = 0;
    while(i < len){
        if(buf[i] == ' '){
            return i;
        }
        i++;
    }
    return -1;
}


jes_err_t __cli_init(void){
    if(is_init) return 0;
    cli_lock = xSemaphoreCreateMutex();
    if(cli_lock == NULL) return e_err_mem_null;
    int32_t stat;
    stat = uart_unif_init(CLI_BAUDRATE, CLI_BUF_SIZE, CLI_BUF_SIZE, (void*)&queue_uart);
    if(stat != 0) return e_err_driver_fail;
    is_init = 1;
    return e_err_no_err;
}

void __cli_close_sess(void){
    uart_event_t ev;
    ev.size = 1;
    ev.type = __extraUART_CLOSE_SESS;
    xQueueSend(queue_uart, &ev, portMAX_DELAY);
}

void cli_server(void *pvParameters){
    char raw_str[__MAX_JOB_STR_LEN_BYTE] = {0};
    char* arg_str = NULL;
    job_struct_t* pj_to_do = NULL;
    uart_event_t event;

    while(1) {
        if (xQueueReceive(queue_uart, (void *)&event, (TickType_t)portMAX_DELAY)) {
            switch (event.type) {
            case UART_DATA:
                uart_unif_read(raw_str, event.size, portMAX_DELAY);
                break;
            case UART_FIFO_OVF:
                // fallthrough to UART_BUFFER_FULL
            case UART_BUFFER_FULL:
                uart_unif_flush();
                xQueueReset(queue_uart);
                pj_to_do = __job_get_job_by_name(CLI_SERVER_NAME);
                pj_to_do->caller = e_origin_core;
                pj_to_do->error = e_err_too_long;
                __core_notify(pj_to_do, 0);
                continue;
            case UART_BREAK:
                // NYI
                break;
            case UART_PARITY_ERR:
                // NYI
                break;
            case UART_FRAME_ERR:
                // NYI
                break;
            case __extraUART_CLOSE_SESS:
                __cli_reprint_header();
                continue;
            default:
                break;
            }
            if(raw_str[event.size-1] == '\r'){
                raw_str[event.size-1] = 0;
            }
            int16_t ws_i = __get_ws_index(raw_str, __MAX_JOB_STR_LEN_BYTE);
            if(ws_i == 0){
                pj_to_do = __job_get_job_by_name(ERROR_HANDLER_NAME);
                pj_to_do->caller = e_origin_core;
                pj_to_do->error = e_err_leading_whitespace;
            }
            else{
                if(ws_i != -1){
                    raw_str[ws_i] = '\0';
                }
                pj_to_do = __job_get_job_by_name(raw_str); // is only read to '\0'
                if(pj_to_do){
                    pj_to_do->caller = e_origin_cli;
                    if(ws_i == -1){
                        memset(pj_to_do->args, 0, __MAX_JOB_ARGS_LEN_BYTE);
                    }
                    else{
                        arg_str = &raw_str[ws_i+1];
                        pj_to_do->error = e_err_no_err;
                        strncpy(pj_to_do->args, arg_str, __MAX_JOB_ARGS_LEN_BYTE);
                    }
                }
            }
            memset((void*)raw_str, 0, __MAX_JOB_STR_LEN_BYTE);
            __core_notify(pj_to_do, 0);
        }
    }
}
