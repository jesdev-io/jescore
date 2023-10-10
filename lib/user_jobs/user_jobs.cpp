#include "user_jobs.h"
#include <Arduino.h>

void stats(void* p){

}

void help(void* p){
    
}


void led(void* p){
    pinMode(27, OUTPUT);
    static bool act = true;
    if(act){
        digitalWrite(27, HIGH);
    }
    else{
        digitalWrite(27, LOW);
    }
    act = !act;
    vTaskDelete(NULL);
}