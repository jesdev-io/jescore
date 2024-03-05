#include "string.h"
#include "cli.h"
#include "job_driver.h"
#include "base_jobs.h"
#include "job_names.h"
#include "driver/uart.h"
#include "uart.h"


static job_struct_t** job_list = NULL;
static QueueHandle_t queue_uart;


void init_cli(void* p){
    job_list = (job_struct_t**)p;
    uart_unif_init(BAUDRATE, CLI_BUF_SIZE, CLI_BUF_SIZE, (void*)&queue_uart);
    __job_register_job(SERIAL_READ_NAME, 4096, 1, cli_server, true, e_role_core);
    __job_register_job(PRINT_JOB_NAME, 4096, 1, __base_job_echo, false, e_role_base);
    job_struct_t* pj_to_do = __job_get_job_by_name(SERIAL_READ_NAME);
    pj_to_do->caller = e_origin_core;
    pj_to_do->is_loop = true;
    __job_notify(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, false);
}

void cli_server(void *pvParameters)
{
    char raw_str[__MAX_JOB_NAME_LEN_BYTE * 2] = {0};
    char* cmd_str = &raw_str[0];
    char* arg_str = NULL;
    job_struct_t* pj_to_do = NULL;
    uart_event_t event;

    while(1) {
        if (xQueueReceive(queue_uart, (void *)&event, (TickType_t)portMAX_DELAY)) {
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
                __job_notify(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, false);
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
                pj_to_do->error = e_err_leading_whitespace;
            }
            else{
                if(ws_i != -1){
                    raw_str[ws_i] = '\0';
                }
                pj_to_do = __job_get_job_by_name(cmd_str);
                if(pj_to_do){
                    pj_to_do->caller = e_origin_cli;
                    if((uint16_t)ws_i < (event.size-1)){
                        arg_str = &raw_str[ws_i+1];
                        strcpy(pj_to_do->args, arg_str);
                    }
                }
            }
            memset((void*)raw_str, 0, __MAX_JOB_NAME_LEN_BYTE * 2);
            __job_notify(__job_get_job_by_name(CORE_JOB_NAME), pj_to_do, false);
        }
    }
}


void reprint_header(void* p){
    uart_unif_write((uint8_t*)CLI_HEADER, 13);
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