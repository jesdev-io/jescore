#include <Arduino.h>
#include "core.h"
#include "user_jobs.h"

void setup() {
    core_init();

    core_register_job("help", 1024, 2, help);
    core_register_job("stats", 1024, 2, stats);
    core_register_job("led", 2048, 1, led);
}

void loop() {

}
