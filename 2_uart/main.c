#include <stdint.h>
#include "gpio.h"
#include "uart.h"

int main() {

    uart_init();
    uart_send('A');


}