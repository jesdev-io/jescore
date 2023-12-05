#include <Arduino.h>
#include "jescore_api.h"

#define LED_PIN BUILTIN_LED

bool act = false;

void stats(void* p){
    to_printer("Not yet implemented!");
}

void help(void* p){
    to_printer("Not yet implemented!");
}

void led_init(void* p){
    pinMode(LED_PIN, OUTPUT);
    act = true;
}

void led_deinit(void* p){
    act = false;
}

void blink(void* p){
    while(1){
        if(act){
            digitalWrite(LED_PIN, HIGH);
            delay(1000);
            digitalWrite(LED_PIN, LOW);
            delay(1000);
        }
        else{
            return;
        }
    }
}



void setup() {
    jes_init();

    register_job("help", 1024, 2, help, false);
    register_job("stats", 1024, 2, stats, false);
    register_job("ledinit", 2048, 1, led_init, false);
    register_job("leddeinit", 1024, 1, led_deinit, false);
    register_job("blink", 2048, 1, blink, true);
}

void loop() {

}
