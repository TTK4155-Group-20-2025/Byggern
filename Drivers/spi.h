#include "avr/io.h"
#include "uart.h"
#include <stdio.h>
#include <stdint.h>

void spi_master_init();
void enable_slave_OLED();
void disable_slave_OLED();
void enable_slave_IO_BOARD();
void disable_slave_IO_BOARD();
void spi_master_transmit(uint8_t letter);
uint8_t spi_master_receive();