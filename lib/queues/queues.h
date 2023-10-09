#ifndef _QUEUES_H_
#define _QUEUES_H_

#include <Arduino.h>
#include "err.h"

extern QueueHandle_t cli_queue;

err_t queue_init();


err_t cli_queue_send(char* item);

#endif