#include <avr/io.h>
#include <stdio.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
#include "Drivers/adc.h"
#include "Drivers/spi.h"

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
// #define ptr ((char)* 0x1000)

int main() {
    uart_init(MYUBRR);
    uart_printf_scanf_init();
    sram_init();
    adc_init();

    position_t JoyStick;
    pos_calibrate(&JoyStick);

    sei();
    int16_t a = 0;
    
    while (1) {
        a = adc_read_TC(0); //Must be called everytime
        pos_read(&JoyStick);
        direction_read(&JoyStick);
    }

    return 0;
}