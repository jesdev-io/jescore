#include <Arduino.h>
#include "jescore.h"

void stats(void* p){
    to_printer("Not yet implemented!");
}

void help(void* p){
    to_printer("Not yet implemented!");
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
}


void setup() {
    jes_init();

    register_job("help", 1024, 2, help);
    register_job("stats", 1024, 2, stats);
    register_job("led", 2048, 1, led);
}

void loop() {

}
