#include <Arduino.h>
#include "jescore.h"

#define MAX_JOB_ARGS_LEN_BYTE 64
#define LED_PIN 4


void led_init(void* p){
    pinMode(LED_PIN, OUTPUT);
}


void blink(void* p){
    static bool act = false;
    act = !act;
    while(act){
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
        delay(1000);
    }
}


void setup() {
    jes_init();
    register_and_launch_job("ledinit", 2048, 1, led_init, false);
    register_and_launch_job("blink", 2048, 1, blink, true);
}

void loop() {

}
