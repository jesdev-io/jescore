#include "example_selector.h"
#ifdef EXAMPLE_BLINK

#include <Arduino.h>
#include "jescore.h"

#define MAX_JOB_ARGS_LEN_BYTE 64
#define LED_PIN 4


void blink(void* p){
    static uint8_t act = 0;
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
    pinMode(LED_PIN, OUTPUT);
    register_and_launch_job("blink", 2048, 1, blink, 1);
}

void loop() {

}

#endif