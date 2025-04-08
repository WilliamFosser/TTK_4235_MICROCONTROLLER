#include "uart.h"
#include "gpio.h"


#define UART ((NRF_UART_REG*)0x40002000)

typedef struct {
    volatile uint32_t TASKS_STARTRX;    // 0x000 Start UART receiver
    volatile uint32_t TASKS_STOPRX;     // 0x004 Stop UART receiver
    volatile uint32_t TASKS_STARTTX;    // 0x008 Start UART transmitter
    volatile uint32_t TASKS_STOPTX;     // 0x00C Stop UART transmitter
    volatile uint32_t RESERVED0[3];     // Reserved space
    volatile uint32_t TASKS_SUSPEND;    // 0x01C Suspend UART
    volatile uint32_t RESERVED1[56];    // Reserved space
    volatile uint32_t EVENTS_CTS;       // 0x100 CTS is activated (set low). Clear To Send.
    volatile uint32_t EVENTS_NCTS;      // 0x104 CTS is deactivated (set high). Not Clear To Send.
    volatile uint32_t EVENTS_RXDRDY;    // 0x108 Data received in RXD
    volatile uint32_t RESERVED2[4];     // Reserved space
    volatile uint32_t EVENTS_TXDRDY;    // 0x11C Data sent from TXD
    volatile uint32_t RESERVED3[1];     // Reserved space
    volatile uint32_t EVENTS_ERROR;     // 0x124 Error detected
    volatile uint32_t RESERVED4[7];     // Reserved space
    volatile uint32_t EVENTS_RXTO;      // 0x144 Receiver timeout
    volatile uint32_t RESERVED5[46];    // Reserved space
    volatile uint32_t SHORTS;           // 0x200 Shortcut register
    volatile uint32_t RESERVED6[64];    // Reserved space
    volatile uint32_t INTENSET;         // 0x304 Enable interrupt
    volatile uint32_t INTENCLR;         // 0x308 Disable interrupt
    volatile uint32_t RESERVED7[93];    // Reserved space
    volatile uint32_t ERRORSRC;         // 0x480 Error source
    volatile uint32_t RESERVED8[31];    // Reserved space
    volatile uint32_t ENABLE;           // 0x500 Enable UART
    volatile uint32_t RESERVED9[1];     // Reserved space
    volatile uint32_t PSELRTS;          // 0x508 Pin select for RTS
    volatile uint32_t PSELTXD;          // 0x50C Pin select for TXD
    volatile uint32_t PSELCTS;          // 0x510 Pin select for CTS
    volatile uint32_t PSELRXD;          // 0x514 Pin select for RXD
    volatile uint32_t RXD;              // 0x518 RXD register
    volatile uint32_t TXD;              // 0x51C TXD register
    volatile uint32_t RESERVED10[1];    // Reserved space
    volatile uint32_t BAUDRATE;         // 0x524 Baud rate
    volatile uint32_t RESERVED11[17];   // Reserved space
    volatile uint32_t CONFIG;           // 0x56C Configuration of parity and hardware flow control
} NRF_UART_REG;

void uart_init() {
    // Configure GPIO-pins
	GPIO->PIN_CNF[6] = 3; // Output - 3 activate input buffer 
    GPIO->PIN_CNF[8] = 2;  // Input

    // Bruke uart
    UART->PSELTXD = 6; //Usikker på riktig kanskje bare pin number
    UART->PSELRXD = 8;
    
    // Set baud rate (9600)
    UART->BAUDRATE = 0x00275000; // 9600 baud
    
    //Ikke CTS eller RTS
    UART->PSELRTS = 0xFFFFFFFF; //RTS = PIN 5, Disconnect = 1 på bit 31
    UART->PSELCTS = 0xFFFFFFFF; //CTS = PIN 7, ^
    

    UART->ENABLE = 4;
    UART->TASKS_STARTRX = 1;
}

void uart_send(char data){

    UART->EVENTS_TXDRDY = 0;


    UART->TASKS_STARTTX = 1;
    UART->TXD = data;


    while (!UART->EVENTS_TXDRDY);

    // Visuals

    // GPIO->PIN_CNF[13] = (3 << 2);
	// GPIO->PIN_CNF[14] = (3 << 2);

    // if (UART->EVENTS_TXDRDY) {
    //     for(int i = 17; i <= 21; i++){
    //         GPIO->DIRSET = (1 << i);
    //         GPIO->OUTCLR = (1 << i);
    //     }
    // }

    UART->EVENTS_TXDRDY = 0;
    UART->TASKS_STOPTX = 1;
};

char uart_read() {
    if (UART->EVENTS_RXDRDY == 1) {
        // Clear event first
        UART->EVENTS_RXDRDY = 0;

        // Les data
        return (char)UART->RXD;
    }
    else {
        return '\0';
    }
};