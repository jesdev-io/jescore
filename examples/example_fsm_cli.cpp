#include <Arduino.h>
#include "jescore.h"
#include <string.h>

#define LED_PIN 4

static bool is_on = false;
static bool processing = false;

void lights(void* p){
    /* 
    This function will be executed when the first word of your
    CLI command is "lights", as registered in line 60 below. All
    other words that follow, such as "on" or "off" will pe passed
    as args to this job and can be retrieved as full string with
    job_get_args().
    */
    char* args = job_get_args();
    char* arg = strtok(args, " ");
    if (!arg){
        uart_unif_write("No argument specified! Use <on> or <off>\n\r");
        return;
    }
    if (strcmp(arg, "on") == 0){
        if(is_on) return; // nothing to do
        if(processing) return; // blocked by lights_off
        processing = true;
        for(uint8_t i = 0; i < 255; i++){
            analogWrite(LED_PIN, i);
            jes_delay_job_ms(5);
        }
        is_on = true;
        processing = false;
    }
    else if (strcmp(arg, "off") == 0){
        if(!is_on) return; // nothing to do
        if(processing) return; // blocked by lights_off
        processing = true;
        for(uint8_t i = 255; i > 0; i--){
            analogWrite(LED_PIN, i);
            jes_delay_job_ms(5);
        }
        is_on = false;
        processing = false;
    }
    else if (strcmp(arg, "state") == 0){
        uart_unif_writef("lights are %s\n\r",(is_on ? "on" : "off"));
    }
    else{
        uart_unif_write("unknown argument!\n\r");
    }
}

void setup() {
    jes_init();
    pinMode(LED_PIN, OUTPUT);
    register_job("lights", 2048, 1, lights, false);
}

void loop() {
    // nothing to do here!
    // jescore is now purely controlled by the CLI
    // call `jescore lights on` to see the LED fade on
    // call `jescore lights status` to get a print
    // of the current status of the LED FSM
}
