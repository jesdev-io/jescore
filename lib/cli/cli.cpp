#include "string.h"
#include "cli.h"
#include "jobs.h"
#include "queues.h"


static job_struct_t** job_list = NULL;

// interrupt service routine for incoming messages
void serialISR(void) {
    detachInterrupt(digitalPinToInterrupt(RX_PIN));
    cmd_struct_t c;
    c.origin = e_origin_interrupt;
    c.content = e_cmd_generic;
    c.o_value = 0;
    job_notify(__get_job(job_list, "core"), c, true);
}

// helper function
void printBootMessage(void){
    Serial.println(BOOT_MSG);
}

// helper function
void printCLIHead(void){
    Serial.print(CLI_HEADER);
}

// processing task (waits for input)
void processInput(void* param) {
    job_struct_t* pj = (job_struct_t*)param;
    job_list = &pj;
    Serial.begin(BAUDRATE);
    Serial.setTimeout(10);
    attachInterrupt(digitalPinToInterrupt(RX_PIN), serialISR, FALLING);
    char inputString[CLI_BUF_SIZE] = {0};   // buffer for incoming messages
    uint32_t inputIndex = 0;                // current index pointing to a letter in the message
    bool inputComplete = false;             // flag for reaching the end of the message
    printBootMessage();
    printCLIHead();
    while(true){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    // wait for notification from interrupt
        vTaskDelay(10 / portTICK_PERIOD_MS);        // wait for all messages to arrive
        while (Serial.available()) {                // read letters
            char c = Serial.read();
            if(c == '\r' || c == '\n'){             // exit on carriage return or newline
                inputString[inputIndex] = '\0';     // terminate string
                inputIndex = 0;                     // reset index
                inputComplete = true;               // set flag
            } 
            else if(inputIndex < CLI_BUF_SIZE - 1){ // append data to buffer if it still has space left
                inputString[inputIndex++] = c;
            }
            
        }
        if(inputComplete){
            cli_queue_send(inputString);
            parseInput(inputString);
            inputComplete = false;
            printCLIHead();
            attachInterrupt(digitalPinToInterrupt(RX_PIN), serialISR, FALLING);
        }
    }
}

void parseInput(char* input_string){
    char* command = strtok(input_string, " ");
    if (command != NULL) {

        // help command
        if(strcmp(command, "help") == 0){
            Serial.println("commands available:");
            Serial.println("\t<button>\t\ttoggle the virtual button");
            Serial.println("\t<switch> <on> <off>\tturn virtual switch on or off");
        }
        // button command
        else if(strcmp(command, "button") == 0){
            Serial.println("BUTTON service routine");
            // toggle a GPIO or whatever...
        }
        // switch command
        else if(strcmp(command, "switch") == 0){
            char* arg = strtok(NULL, " ");
            if(arg != NULL){
                // switch on command
                if(strcmp(arg, "on") == 0){
                    Serial.println("SWITCH ON service routine");
                    // toggle a GPIO or whatever...
                }
                // switch off command
                else if(strcmp(arg, "off") == 0){
                    Serial.println("SWITCH OFF service routine");
                    // toggle a GPIO or whatever...
                }
                // warn user about not specifying an argument
                else{
                    Serial.printf("Unknown argument <%s>. Use <on> or <off>\r\n.", arg);
                }
            }
            // warn user about an unknown argument
            else{
                Serial.println("Missing argument. Use <on> or <off>.");
            }
        }
        // warn user about unknown command
        else{
            Serial.printf("Unknown command <%s>\r\n", command);
        }
    }
}

