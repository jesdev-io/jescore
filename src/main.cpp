#include <Arduino.h>
#include "jescore.h"

#define LED_PIN BUILTIN_LED

void stats(void* p){
    to_printer("Not yet implemented!");
}

void help(void* p){
    to_printer("Not yet implemented!");
}

void led_init(void* p){
    pinMode(LED_PIN, OUTPUT);
}

void led(void* p){
    static bool act = true;
    if(act){
        digitalWrite(LED_PIN, HIGH);
    }
    else{
        digitalWrite(LED_PIN, LOW);
    }
    act = !act;
}


void setup() {
    jes_init();

    register_job("help", 1024, 2, help);
    register_job("stats", 1024, 2, stats);
    register_job("ledinit", 2048, 1, led_init);
    register_job("led", 2048, 1, led);
}

void loop() {

}
