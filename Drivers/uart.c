//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "uart.h"

volatile uint8_t tx_ready = 0;
volatile uint8_t rx_buffer[8];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void uart_init(unsigned int ubrr) {
    tx_ready = 1;

    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0B |= (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
}

int uart_send(uint8_t letter, FILE *stream) {
    if (tx_ready) {
        if (letter == '\n') {
            uart_send('\r', stream);
        }
        tx_ready = 0;
        UDR0 = letter;
    } return 0;
}

uint8_t uart_read(FILE *stream) {
    uint8_t read_byte = 0;
    while (rx_head == rx_tail);
    read_byte = rx_buffer[rx_head];
    for (int i = rx_head; i < (rx_tail); i++) {
        rx_buffer[i] = rx_buffer[i+1];
    }
    rx_tail -= 1;
    return read_byte;
}



ISR(USART0_UDRE_vect) {
    tx_ready = 1;
}

ISR(USART0_RXC_vect) {
    rx_buffer[rx_tail] = UDR0;
    rx_tail += 1;
}