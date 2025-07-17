#include "unity_config.h"
#include "uart_unif.h"

void unityOutputStart(){
    if (uart_unif_init(115200, 64, 64, NULL) != 0){
        while(1);
    }
}

void unityOutputChar(char c){
    char s[] = {0, 0};
    s[0] = c;
    uart_unif_write(s);
}

void unityOutputFlush() {
    uart_unif_flush();
}

void unityOutputComplete(){
    uart_unif_deinit();
}