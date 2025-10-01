#include <avr/io.h>
#include <stdio.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
#include "Drivers/adc.h"
#include "Drivers/spi.h"
#include "Drivers/oled.h"

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main() {
    uart_init(MYUBRR);
    uart_printf_scanf_init();
    sram_init();
    adc_init();
    spi_master_init();
    oled_init();

    position_t JoyStick;
    pos_calibrate(&JoyStick); // DO NOT REMOVE

    sei();
    int16_t a = 0;

    oled_home();
    oled_print_box(3);
    
    while (1) {
        a = adc_read_TC(0); // DO NOT REMOVE
        //oled_test();
    }

    return 0;
}