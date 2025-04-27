#include "string.h"
#include "cli.h"
#include "job_driver.h"
#include "base_jobs.h"
#include "job_names.h"
#include "driver/uart.h"
#include "uart_unif.h"


static job_struct_t** job_list = NULL;
static QueueHandle_t queue_uart;
static uint8_t open_sess = 0;


uint8_t cli_get_sess_state(void){
    return open_sess;
}

void cli_set_sess_state(uint8_t sess_state){
    open_sess = sess_state;
}

void init_cli(void* p){
    job_list = (job_struct_t**)p;
    uart_unif_init(BAUDRATE, CLI_BUF_SIZE, CLI_BUF_SIZE, (void*)&queue_uart);
    uart_unif_write(BOOT_MSG);
    __job_register_job(SERIAL_READ_NAME, 4096, 1, cli_server, 1, e_role_core);
    __job_register_job(PRINT_JOB_NAME, 4096, 1, __base_job_echo, 0, e_role_base);
    job_struct_t* pj_to_do = __job_get_job_by_name(SERIAL_READ_NAME);
    pj_to_do->caller = e_origin_core;
    pj_to_do->is_loop = 1;
    __job_notify_with_job(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, 0);
}

void cli_server(void *pvParameters)
{
    char raw_str[__MAX_JOB_ARGS_LEN_BYTE] = {0};
    char* arg_str = NULL;
    job_struct_t* pj_to_do = NULL;
    uart_event_t event;

    while(1) {
        if (xQueueReceive(queue_uart, (void *)&event, (TickType_t)portMAX_DELAY)) {
            open_sess = 1;
            switch (event.type) {
            case UART_DATA:
                uart_unif_read((uint8_t*)raw_str, event.size, portMAX_DELAY);
                break;
            case UART_FIFO_OVF:
                uart_unif_flush();
                xQueueReset(queue_uart);
                break;
            case UART_BUFFER_FULL:
                uart_unif_flush();
                pj_to_do = __job_get_job_by_name(ERROR_HANDLER_NAME);
                pj_to_do->error = e_err_too_long;
                __job_notify_with_job(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, 0);
                xQueueReset(queue_uart);
                break;
            case UART_BREAK:
                break;
            case UART_PARITY_ERR:
                break;
            case UART_FRAME_ERR:
                break;
            default:
                break;
            }
            if(raw_str[event.size-1] == '\r'){
                raw_str[event.size-1] = 0;
            }
            int16_t ws_i = __get_ws_index(raw_str, __MAX_JOB_NAME_LEN_BYTE);
            if(ws_i == 0){
                pj_to_do = __job_get_job_by_name(ERROR_HANDLER_NAME);
                pj_to_do->caller = e_origin_cli;
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
                        strcpy(pj_to_do->args, arg_str);
                    }
                }
            }
            memset((void*)raw_str, 0, __MAX_JOB_NAME_LEN_BYTE);
            __job_notify_with_job(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, 0);
        }
    }
}


void reprint_header(void* p){
    uart_unif_write(CLI_HEADER);
}


static inline int16_t __get_ws_index(char* buf, uint16_t len){
    int16_t i = 0;
    while(i < len){
        if(buf[i] == ' '){
            return i;
        }
        i++;
    }
    return -1;
}