#include <Arduino.h>
#include "jescore_api.h"

#define MAX_JOB_ARGS_LEN_BYTE 64

#ifdef BUILTIN_LED
#define LED_PIN BUILTIN_LED
#else
#define LED_PIN 2
#endif

bool act = false;

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
    register_and_launch_job("ledinit", 2048, 1, led_init, false);
    register_job("leddeinit", 1024, 1, led_deinit, false);
    register_and_launch_job("blink", 2048, 1, blink, true);
}

void loop() {

}
