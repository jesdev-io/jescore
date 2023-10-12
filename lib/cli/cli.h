#ifndef _CLI_H_
#define _CLI_H_

#include "Arduino.h"

#define RX_PIN          3
#define CLI_BUF_SIZE    128
#define BAUDRATE        115200


void init_cli(void* p);
void serialISR(void);
void read_serial(void* p);
void __echo(void* p);
static inline int16_t __get_ws_index(char* buf, uint16_t len);

#endif