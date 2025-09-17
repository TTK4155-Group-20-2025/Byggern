//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "uart.h"

#define BUFFER_SIZE 64

volatile uint8_t rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;
volatile uint8_t tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

void uart_init(unsigned int ubrr) {
    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0B |= (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
}

int uart_send(uint8_t letter) {
    // Standard non-ring buffer
    // if (tx_head < BUFFER_SIZE + 1) {
    //     cli();
    //     for (int i = tx_head; i > tx_tail; i--) {
    //         tx_buffer[i] = tx_buffer[i-1];
    //     } tx_head += 1;
    //     tx_buffer[tx_tail] = letter;
    //     sei();
    // } 

    // Ring buffer
    uint8_t next_head = (tx_head + 1) % BUFFER_SIZE;
    while(next_head == tx_tail);
    tx_buffer[tx_head] = letter;
    tx_head = next_head;

    return 0;
}

uint8_t uart_read() {
    uint8_t read_byte = 0;
    while (rx_head == rx_tail);
    read_byte = rx_buffer[rx_head];
    for (int i = rx_head; i < (rx_tail); i++) {
        rx_buffer[i] = rx_buffer[i+1];
    }
    rx_tail -= 1;
    return read_byte;
}

void uart_printf_scanf_init() {
    fdevopen(uart_printf, uart_scanf);
}

int uart_printf(uint8_t letter, FILE *stream) {
    if (letter == '\n') {
        uart_printf('\r', stream);
    } uart_send(letter);
    return 0;
}

uint8_t uart_scanf(FILE *stream) {
    return uart_read();
}

ISR(USART0_UDRE_vect) {
    // Implementation w/o ring buffer
    // if (tx_head > tx_tail) {
    //     UDR0 = tx_buffer[tx_head - 1];
    //     tx_head -= 1;
    // }

    if (tx_head != tx_tail) {
        UDR0 = tx_buffer[tx_tail];
        tx_tail = (tx_tail + 1) % BUFFER_SIZE;
    }
}

ISR(USART0_RXC_vect) {
    rx_buffer[rx_tail] = UDR0;
    rx_tail += 1;
}