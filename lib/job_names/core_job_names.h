#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CORE_JOB_NAMES_H_
#define _CORE_JOB_NAMES_H_

#define CORE_JOB_NAME       "core"
#define ERROR_HANDLER_NAME  "errorhandler"
#ifndef JES_DISABLE_CLI
#define INIT_CLI_JOB_NAME   "initcli"
#define SERIAL_READ_NAME    "cliserver"
#define HEADER_PRINTER_NAME "headerprint"
#define HELP_NAME           "help"
#define STATS_NAME          "stats"
#endif

#endif

#ifdef __cplusplus
}
#endif