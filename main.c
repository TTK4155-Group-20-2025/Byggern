#include <avr/io.h>
#include "Drivers/uart.h"
#include <stdio.h>

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main() {
    uart_init(MYUBRR);
    sei();
    fdevopen(uart_send, uart_read);
    printf("Hei pa");

    DDRA = (1 << DDA0);
    uint8_t testValue = 'c';
    while (1) {


        // //uart_read();
        // PORTA = (1 << PA0);
        // //uart_send(testValue);
        // if (uart_read()) {
        //     PORTA = (0 << PA0);
        // }
        // uart_send(testValue);

        // for (int i = 0; i < 25000; i++) {
        // }

        // //PORTA = (0 << PA0);

        // //for (int i = 0; i < 25000; i++) {
        // //}
    }
    return 0;
}