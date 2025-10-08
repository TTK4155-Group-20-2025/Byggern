#include <avr/io.h>
#include <stdio.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
//#include "Drivers/adc.h"
#include "Drivers/spi.h"
#include "Drivers/oled.h"
#include "Drivers/menu.h"

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
    menu_t my_menu;
    pos_calibrate(&JoyStick); // DO NOT REMOVE
    menu_init(&my_menu);

    sei();
    int16_t a = 0;
    
    while (1) {
        // a = adc_read_TC(0); // DO NOT REMOVE
        oled_update(); // DO NOT REMOVE
        update_menu(&my_menu, &JoyStick);
    }

    return 0;
}