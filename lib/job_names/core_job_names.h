#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CORE_JOB_NAMES_H_
#define _CORE_JOB_NAMES_H_

#define CORE_JOB_NAME       "core"
#define ERROR_HANDLER_NAME  "errh"
#ifndef JES_DISABLE_CLI
#define CLI_SERVER_NAME    "clisrv"
#if __JES_LOG_LEN > 0
#define LOG_PRINTER_NAME    "logp"
#endif // __JES_LOG_LEN > 0
#endif

#endif

#ifdef __cplusplus
}
#endif