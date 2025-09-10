#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void uart_init(unsigned int ubrr); // Intializes the UART
int uart_send(uint8_t letter); // The UART sends a char (8-bit signal) via. the RS-232
uint8_t uart_read(); // The UARTS reads a char (8-bit signal) from the terminal via. the RS-232 

void uart_printf_scanf_init(); // Initializes printf functionality
int uart_printf(uint8_t letter, FILE *stream); // Prints a stream of char signals using the uart_send function
uint8_t uart_scanf(FILE *stream); //