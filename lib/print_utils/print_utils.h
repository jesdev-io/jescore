#ifndef _PRINT_UTILS_H_
#define _PRINT_UTILS_H_

#include <Arduino.h>

#ifndef BOOT_MSG
#define BOOT_MSG        "\n************\n\rjes-core CLI\n\r************\n\r"
#endif
#ifndef CLI_HEADER
#define CLI_HEADER      "jes-core $ "
#endif


void __cli_output_serial(const char* s);


#endif