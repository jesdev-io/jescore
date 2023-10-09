#include "user_jobs.h"
#include <Arduino.h>

void stats(void* p){

}

void help(void* p){
    Serial.print("This is helpful");
    vTaskDelete(NULL);
}