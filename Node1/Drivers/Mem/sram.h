#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

void sram_init(); // Intializes the SRAM
void SRAM_test(void); // Code from the manual for testing the SRAM