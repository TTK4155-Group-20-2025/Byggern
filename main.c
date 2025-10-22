#include <avr/io.h>
#include <stdio.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
//#include "Drivers/adc.h"
#include "Drivers/spi.h"
#include "Drivers/oled.h"
#include "Drivers/menu.h"
#include "Drivers/can.h"

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
    can_init();

    position_t JoyStick;
    message_t message;
    message.ID = NODE1;
    message.datalength = 1;
    message.data[0] = 0b101;
    message.buffer = TX0;
    menu_t my_menu;
    pos_calibrate(&JoyStick); // DO NOT REMOVE
    menu_init(&my_menu);

    sei();
    
    while (1) {
        // a = adc_read_TC(0); // DO NOT REMOVE
        oled_update(); // DO NOT REMOVE
        update_menu(&my_menu, &JoyStick);
        can_receive(&message);
    }

    return 0;
}