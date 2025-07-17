#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CLI_H_
#define _CLI_H_

#include <inttypes.h>

#define CLI_BUF_SIZE    256
#define CLI_BAUDRATE        115200

#ifndef CLI_BOOT_MSG
#define CLI_BOOT_MSG        "\n************\n\rjescore CLI\n\r************\n\r"
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

/// @brief Get the session state of the CLI (open or closed).
/// @return Open (1) or closed (0).
uint8_t __cli_get_sess_state(void);

/// @brief Get the session state of the CLI (open or closed).
/// @param sess_state State to be set: open (1) or closed (0).
void __cli_set_sess_state(uint8_t sess_state);

/// @brief CLI initializer.
/// @return status, `e_err_no_err` if OK.
jes_err_t __cli_init(void);

/// @brief CLI serving job.
/// @param p Mandatory args pointer.
/// @note This job is started by the core if JES_DISABLE_CLI is not defined.
void cli_server(void* p);

/// @brief Reprint the CLI header.
/// @returns status, `e_err_no_err` if OK.
/// @note This is neccessary to determine if a transaction has finished on the 
///       client side.
jes_err_t __cli_reprint_header(void);

/// @brief Get the index of the first white space in a string.
/// @param buf String to analyze.
/// @param len Length of string.
/// @return Index of white space in string. Returns -1 for the absence of a white space.
int16_t __get_ws_index(char* buf, uint16_t len);

#endif

#ifdef __cplusplus
}
#endif