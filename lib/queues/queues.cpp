#include "queues.h"
#include "jobs.h"

QueueHandle_t cli_queue;

err_t cli_queue_init(){
    cli_queue = xQueueCreate(__MAX_JOB_NAME_LEN_BYTE, sizeof(char));
    if(cli_queue == NULL){ return e_mem_null; }
    return e_no_err;
}

err_t cli_queue_send(char* item, bool from_ISR){
    BaseType_t stat;
    if(from_ISR){
        BaseType_t dummy = pdFALSE;
        stat = xQueueSendFromISR(cli_queue, &item, &dummy);
    }
    else{
        stat = xQueueSend(cli_queue, &item, portMAX_DELAY);
    }
    if(stat != pdTRUE){
        return e_peripheral_block;
    }
    return e_no_err;
}

err_t cli_queue_receive(char* buf){
    BaseType_t stat = xQueueReceive(cli_queue, &buf, portMAX_DELAY);
    if(stat != pdTRUE){
        return e_peripheral_block;
    }
    return e_no_err;
}