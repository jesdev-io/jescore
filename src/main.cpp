#include <Arduino.h>
#include "jescore.h"
#include "user_jobs.h"

void setup() {
    jes_init();

    register_job("help", 1024, 2, help);
    register_job("stats", 1024, 2, stats);
    register_job("led", 2048, 1, led);
}

void loop() {

}
