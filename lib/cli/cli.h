#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CLI_H_
#define _CLI_H_

#include <inttypes.h>

#define CLI_BUF_SIZE    256
#define CLI_BAUDRATE    115200

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

/// @brief CLI initializer. Only accesses hardware, buffers and variables,
///        no jobs.
/// @return status, `e_err_no_err` if OK.
jes_err_t __cli_init(void);

/// @brief Send a __extraUART_CLOSE_SESS command to the CLI server
///        to terminate an open CLI session.
void __cli_close_sess(void);

/// @brief CLI serving job.
/// @param p Mandatory args pointer.
/// @note This job is started by the core if JES_DISABLE_CLI is not defined.
void cli_server(void* p);

#endif

#ifdef __cplusplus
}
#endif