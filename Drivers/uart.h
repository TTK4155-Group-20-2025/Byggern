#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void uart_init(unsigned int ubrr);
int uart_send(uint8_t letter, FILE *stream);
uint8_t uart_read(FILE *stream);