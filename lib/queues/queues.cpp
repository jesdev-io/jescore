#include "queues.h"
#include "jobs.h"

QueueHandle_t cli_queue;

err_t cli_queue_init(){
    cli_queue = xQueueCreate(MAX_JOB_NAME_LEN_BYTE, sizeof(char));
    if(cli_queue == NULL){ return e_mem_null; }
    return e_no_err;
}

err_t cli_queue_send(char* item){
    BaseType_t stat = xQueueSend(cli_queue, &item, portMAX_DELAY);
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