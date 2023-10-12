#include "print_utils.h"
#include "Arduino.h"

static void __cli_output_serial(const char* s){
    Serial.println(s);
    Serial.print(CLI_HEADER);
}