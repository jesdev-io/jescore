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
static int16_t __cli_get_ws_index(char* buf, uint16_t len){
    int16_t i = 0;
    while(i < len){
        if(buf[i] == ' '){
            return i;
        }
        i++;
    }
    return -1;
}

/// @brief Strip whitespaces and delimit string.
/// @param inp Input array, raw string.
/// @param len Length of valid input, at most length of array.
/// @return Pointer to sanitized substring in original array.
/// @note len is the string length without '\0'.
static char* __cli_sanitize_input(char* inp, uint32_t len){
    if (inp == NULL || len == 0) return NULL;
    char* front = inp;
    char* back = &inp[len-1];
    while(*back == ' ' || *back == '\r'){
        *back = 0;
        back--;
    }
    while(*front == ' '){
        front++;
    }
    return front;
}

/// @brief Split the command string from the argstring in a string.
/// @param inp Input buffer.
/// @param len Length of input.
/// @return Pointer to arguments.
/// @note After this call, inp is equivalent to the command string.
/// @note Call `__cli_sanitize_input()` before this!
static char* __cli_cmd_arg_split(char* inp, uint32_t len){
    if(inp == NULL || len == 0) return NULL;
    int16_t ws_i = __cli_get_ws_index(inp, len);
    if(ws_i == -1){
        return NULL;
    }
    char* args = __cli_sanitize_input(&inp[ws_i], len - ws_i);
    inp[ws_i] = 0;
    return args;
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

void cli_server(void *p){
    job_struct_t* pself = (job_struct_t*)p;
    char raw_str[__MAX_JOB_STR_LEN_BYTE] = {0};
    job_struct_t* pj_to_do = NULL;
    uart_event_t event;

    while(1) {
        if (xQueueReceive(queue_uart, (void *)&event, (TickType_t)portMAX_DELAY)) {
            switch (event.type) {
            case UART_DATA:
                uart_unif_read(raw_str, event.size, portMAX_DELAY);
                // skip nonsense or empty input
                if(!raw_str[0] || raw_str[0] == '\r'){
                    __cli_reprint_header();
                    continue;
                }
                break;
            case UART_FIFO_OVF:
                // fallthrough to UART_BUFFER_FULL
            case UART_BUFFER_FULL:
                uart_unif_flush();
                xQueueReset(queue_uart);
                pj_to_do = pself;
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
            char* clean = __cli_sanitize_input(raw_str, event.size);
            char* cmd = clean;
            char* args = __cli_cmd_arg_split(clean, event.size);
            pj_to_do = __job_get_job_by_name(cmd);
            if(pj_to_do){
                pj_to_do->caller = e_origin_cli;
                if(args){
                    pj_to_do->error = e_err_no_err;
                    strncpy(pj_to_do->args, args, __MAX_JOB_ARGS_LEN_BYTE);
                }
                else{
                    memset(pj_to_do->args, 0, __MAX_JOB_ARGS_LEN_BYTE);
                }
            }
            memset((void*)raw_str, 0, __MAX_JOB_STR_LEN_BYTE);
            __core_notify(pj_to_do, 0);
        }
    }
}
