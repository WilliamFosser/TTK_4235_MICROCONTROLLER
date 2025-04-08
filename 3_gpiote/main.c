#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

#define BUTTON_GPIO_NUMBER 13
#define LED_START_GPIO_NUMBER 17


void LED_init() {
    for(int i = LED_START_GPIO_NUMBER; i <= 21; i++){
        GPIO->DIRSET = (1 << i);
        GPIO->OUTSET = (1 << i);
    }
}

void gpio_init(){ 
	GPIO->PIN_CNF[13] =  (1 << 1) | (3 << 2);

}


void config_button() {
    GPIOTE->CONFIG[0] = (1 /*RISING EDGE*/ << 16) | (BUTTON_GPIO_NUMBER << 8) | 1; 
}

void led_config() {
    for (int i = 0; i < 5; i++) {
        GPIOTE->CONFIG[1+i] = (3 /*TOGGLE*/ << 16) | ((LED_START_GPIO_NUMBER + i) << 8) | 3; 
    }
}

void config_ppi() {
    // Kobler knapp-event til alle LED-toggle-tasks
    // GPIOTE Event: IN[0]
    // GPIOTE Tasks: OUT[1], OUT[2], OUT[3], OUT[4], OUT[5]
    
    // Setter opp 5 PPI-kanaler (0-4)
    for (int i = 0; i < 5; i++) {
        // Event endpoint: Knapp 1 trykket
        PPI->PPI_CH[i].EEP = (uint32_t)&(GPIOTE->EVENTS_IN[0]);
        
        // Task endpoint: Toggle LED pin
        PPI->PPI_CH[i].TEP = (uint32_t)&(GPIOTE->TASKS_OUT[i+1]);
        
        // Aktiverer PPI-kanalen
        PPI->CHENSET = (1 << i);
    }
}




int main() {
    LED_init();
    gpio_init();
    config_button();
    led_config();

    config_ppi();



    while(1) {
        
    }

}