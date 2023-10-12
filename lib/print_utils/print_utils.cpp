#include "print_utils.h"

void __cli_output_serial(const char* s){
    Serial.println(s);
    Serial.print(CLI_HEADER);
}