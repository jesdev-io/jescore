#include <Arduino.h>
#include "jescore.h"
#include <inttypes.h>

#define LED_PIN 5

static uint32_t blink_pause = 1000;

void blink_forever(void* p){
    /*
    This function is a placeholder for a  "time critical" loop 
    that does not have the capability of sampling user input. 
    This is the "sync"-job that we want to steer with the "async"-
    job. This job is an infinite, synchronous loop.
    */
    while(1){
        digitalWrite(LED_PIN, 1);
        jes_delay_job_ms(blink_pause);
        digitalWrite(LED_PIN, 0);
        jes_delay_job_ms(blink_pause);
    }
}

void blink_switch(void* p){
    /*
    This function handles "async" user input and applies it to
    the time critical job. In this very simple example, the shared
    resource is a single uint32_t variable "blink_pause". This job
    handles asynchronous user input and then exits.
    */
    char* arg = jes_job_arg_next();
    if(!arg){
        uart_unif_write("Usage: switch <time in ms>\r\n");
        return;
    }
    while(arg != NULL){
        blink_pause = atoi(arg);
        uart_unif_writef("Blink rate set to %s ms\n\r", arg);
        arg = jes_job_arg_next();
    }
}

void setup() {
    jes_init();
    pinMode(LED_PIN, OUTPUT);
    jes_register_job("switch", 2048, 1, blink_switch, false);
    jes_register_and_launch_job("blink", 2048, 1, blink_forever, true);
}

void loop() {
    // nothing to do here!
    // call `jescore switch <time in ms>` to see the LED change blink speed
}