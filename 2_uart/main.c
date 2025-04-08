#include <stdint.h>
#include "gpio.h"
#include "uart.h"
#include <stdio.h>
#include <sys/types.h> // For ssize_t


void button_init(){ 
	GPIO->PIN_CNF[13] = (3 << 2);
	GPIO->PIN_CNF[14] = (3 << 2);
}

void LED_init() {
    for(int i = 17; i <= 21; i++){
        GPIO->DIRSET = (1 << i);
        GPIO->OUTSET = (1 << i);
    }
}

void toggle_LEDs() {
    for(int i = 17; i <= 21; i++){
        if (GPIO->OUT & (1 << i)) {
            GPIO->OUTCLR = (1 << i);
        }
        else {
            GPIO->OUTSET = (1 << i);
        }
    }
}



ssize_t _write(int fd, const void *buf, size_t count){
    char * letter = (char *)(buf);
    for(int i = 0; i < count; i++){
        uart_send(*letter);
        letter++;
    }

    return count;
}



int main() {

    button_init();
    LED_init();
    uart_init();
    


    int sleep = 0;
    int prev_button1 = 1;
    int prev_button2 = 1;


    iprintf("The average grade in TTK%d was in %d was: %c\n\r",4235,2022,'B');

    while(1) {

        int button1 = (GPIO->IN & (1 << 13)) ? 1 : 0;
        int button2 = (GPIO->IN & (1 << 14)) ? 1 : 0;
        
        // Knapp-press detektert (falling edge)
        if (prev_button1 && !button1) {
            uart_send('A');
        }
        
        if (prev_button2 && !button2) {
            uart_send('B');
        }
        
        prev_button1 = button1;
        prev_button2 = button2;



        /*################################################*/
        char rx_data = uart_read();
        if ((rx_data == 'A') | (rx_data == 'B')) {
            toggle_LEDs();
        }
        

        sleep = 10000;
        while(--sleep); // Delay
    }
}