#include "spi.h"

#define BUFFER_SIZE 64

// For spi interrupt
// volatile uint8_t spi_receive_buffer[BUFFER_SIZE];
// volatile uint8_t spi_receive_head = 0;
// volatile uint8_t spi_receive_tail = 0;
// volatile uint8_t spi_transmit_buffer[BUFFER_SIZE];
// volatile uint8_t spi_transmit_head = 0;
// volatile uint8_t spi_transmit_tail = 0;

// volatile uint8_t spi_ready = 0;

void spi_master_init() {
    DDRB |= (1 << PB4) | (1 << PB1); //ADD ANOTHER PORT FOR SS IN CAN EXERCISE
    PORTB |= (1 << PB4) | (1 << PB1);

    SPCR |= (1 << SPE) | (1 << MSTR);
    SPSR |= (1 << SPI2X);

    // SPCR |= (1 << SPIE) // For spi interrupt
    // spi_ready = 1;
}

// Enable OLED as slave
void enable_slave_OLED() {
    if (PORTB & (1 << PB1)) {
        PORTB ^= (1 << PB1);
    } else {
        printf("OLED slave is already enabled\n");
    }
}
void disable_slave_OLED() {
    if (!PORTB & (1 << PB1)) {
        PORTB ^= (1 << PB1);
    } else {
        printf("OLED slave is already disabled\n");
    }
}
// Enable IO Board as slave
void enable_slave_IO_BOARD() {
    if (PORTB & (1 << PB4)) {
        PORTB ^= (1 << PB4);
    } else {
        printf("IO BOARD slave is already enabled\n");
    }
}
void disable_slave_IO_BOARD() {
    if (!PORTB & (1 << PB4)) {
        PORTB ^= (1 << PB4);
    } else {
        printf("IO BOARD slave is already disabled\n");
    }
}
// MAYBE ADD ENABLE/DISABLE FOR CAN

void spi_master_transmit(uint8_t letter) {
    // For spi interrupt
    // if (spi_transmit_head == spi_transmit_tail) {
    //     while(!spi_ready_buffer_empty);
    //     SPDR = letter;
    // }
    // uint8_t next_head = (spi_transmit_head + 1) % BUFFER_SIZE;
    // while(next_head == spi_transmit_tail);
    // spi_transmit_buffer[spi_transmit_head] = letter;
    // spi_transmit_head = next_head;
    SPDR = letter;
    while(!(SPSR & (1 << SPIF)));
}

uint8_t spi_master_receive() {
    // For spi interrupt
    // if (spi_receive_head != spi_receive_tail) {
    //     spi_receive_tail = (spi_receive_tail + 1) % BUFFER_SIZE;
    //     return spi_receive_buffer[spi_receive_tail];
    // }
    while(!(SPSR & (1 << SPIF)));
    return SPDR;
}

// ISR(SPI_STC_vect) {
//     if (spi_transmit_head != spi_transmit_tail) {
//         SPDR = spi_transmit_buffer[spi_transmit_tail];
//         spi_transmit_tail = (spi_transmit_tail + 1) % BUFFER_SIZE;
//         spi_ready_buffer_empty = 0;
//     } else {
//         spi_ready_buffer_empty = 1;
//     }
// }