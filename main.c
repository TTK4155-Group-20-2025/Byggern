#include <avr/io.h>
#include <stdio.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
#include "Drivers/adc.h"

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
// #define ptr ((char)* 0x1000)

int main() {
    uart_init(MYUBRR);
    uart_printf_scanf_init();
    sram_init();
    adc_init();

    position_t position_joystick;
    pos_calibrate(&position_joystick);

    // printf("Hei");

    sei();
    
    // printf("Polsefingre");

    // SRAM_test();
    
    //DDRC |= (1 << PC3) | (1 << PC2);
    // DDRE |= 1 << PE1;

    // PORTE = 1 << PE1;
    //PORTC |= (0 << PC3) | (0 << PC2);

    // PORTE = 0 << PE1;
    // PORTA = 0;

    //volatile char *ext_ram = (char *) 0x1CFF;  // Start address for the SRAM
 
    //uint8_t testValue = 'c';
    while (1) {
        pos_read(&position_joystick);
        //printf("Pos x: %u \n", position_joystick.X);
        printf("Channel 0: %u \n", adc_read(0));

        //*ext_ram = 0;
        //printf("Hei");
        
    //     if (uart_read() == 'a') {
    //         PORTA = (1 << PA0);
    //     }
    //     uart_send(testValue);
    //     if (uart_read() == 'a') {
    //         PORTA = (0 << PA0);
    //     }
    //     uart_send(testValue);

    //     for (int i = 0; i < 25000; i++) {
    //     }
        
    //     //PORTA = (0 << PA0);

    //     //for (int i = 0; i < 25000; i++) {
    //     //}
    }

    return 0;
}