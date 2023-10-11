#include "string.h"
#include "cli.h"
#include "jobs.h"
#include "queues.h"


static job_struct_t** job_list = NULL;


void init_cli(void* p){
    job_list = (job_struct_t**)p;
    Serial.begin(BAUDRATE);
    Serial.setTimeout(10);
    attachInterrupt(digitalPinToInterrupt(RX_PIN), serialISR, FALLING);
    __job_register_job(job_list, "readserial", 4096, 1, read_serial);
    __printBootMessage();
    __printCLIHead();
    vTaskDelete(NULL);
}

void serialISR(void) {
    detachInterrupt(digitalPinToInterrupt(RX_PIN));
    job_struct_t* pj_to_do = __job_get_job(job_list, "readserial");
    pj_to_do->caller = e_origin_interrupt;
    __job_notify(__job_get_job(job_list, "core"), pj_to_do, true);
}


static void __printBootMessage(void){
    Serial.println(BOOT_MSG);
}


static void __printCLIHead(void){
    Serial.print(CLI_HEADER);
}


void read_serial(void* p) {
    static char raw_str[__MAX_JOB_NAME_LEN_BYTE * 2] = {0};
    char* cmd_str = &raw_str[0];
    char* arg_str = NULL;
    uint8_t sep_idx = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    job_struct_t* pj_to_do = NULL;
    vTaskDelay(10 / portTICK_PERIOD_MS);

    while(Serial.available() && i < __MAX_JOB_NAME_LEN_BYTE - 1) {
        char c = Serial.read();
        if(c == '\r' || c == '\n'){
            raw_str[i] = '\0'; // leave this until raw_str is not static
            int16_t ws_i = __get_ws_index(raw_str, __MAX_JOB_NAME_LEN_BYTE);
            if(ws_i == 0){
                Serial.println("Leading whitespace error");
            }
            else{
                if(ws_i != -1){
                    raw_str[ws_i] = '\0';
                }
                pj_to_do = __job_get_job(job_list, cmd_str);
                pj_to_do->caller = e_origin_cli;
                if((uint16_t)ws_i < i){
                    arg_str = &raw_str[ws_i+1];
                    strcpy(pj_to_do->args, arg_str);
                }
                __job_notify(__job_get_job(job_list, "core"), pj_to_do, false);
            }
        }
        else{
            raw_str[i++] = c;
        }
    }
    attachInterrupt(digitalPinToInterrupt(RX_PIN), serialISR, FALLING);
    __printCLIHead();
    vTaskDelete(NULL);
}


static int16_t __get_ws_index(char* buf, uint16_t len){
    int16_t i = 0;
    while(i < len){
        if(buf[i] == ' '){
            return i;
        }
        i++;
    }
    return -1;
}