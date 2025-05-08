#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CLI_H_
#define _CLI_H_

#include <inttypes.h>

#define RX_PIN          3
#define CLI_BUF_SIZE    256
#define BAUDRATE        115200

#ifndef BOOT_MSG
#define BOOT_MSG        "\n************\n\rjescore CLI\n\r************\n\r"
#endif
#ifndef CLI_HEADER
#define CLI_HEADER      "\rjescore $ "
#endif

#define CLR_R   (uint8_t*)"\x1b[31m"
#define CLR_G   (uint8_t*)"\x1b[32m"
#define CLR_Y   (uint8_t*)"\x1b[33m"
#define CLR_B   (uint8_t*)"\x1b[34m"
#define CLR_M   (uint8_t*)"\x1b[35m"
#define CLR_C   (uint8_t*)"\x1b[36m"
#define CLR_Gr  (uint8_t*)"\x1b[90m"
#define CLR_X   (uint8_t*)"\x1b[0m"

uint8_t cli_get_sess_state(void);
void cli_set_sess_state(uint8_t sess_state);
void init_cli(void* p);
void serialISR(void);
void cli_server(void* p);
void reprint_header(void* p);
static inline int16_t __get_ws_index(char* buf, uint16_t len);

#endif

#ifdef __cplusplus
}
#endif