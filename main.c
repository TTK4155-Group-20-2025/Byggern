#include <avr/io.h>
#include "Drivers/uart.h"
#include "Drivers/sram.h"
#include <stdio.h>

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
// #define ptr ((char)* 0x1000)

int main() {
    uart_init(MYUBRR);
    sei();
    uart_printf_scanf_init();
    // printf("Polsefingre");
    sram_init();

    //SRAM_test();
    
    //DDRC |= (1 << PC3) | (1 << PC2);
    // DDRE |= 1 << PE1;

    // PORTE = 1 << PE1;
    //PORTC |= (0 << PC3) | (0 << PC2);

    // PORTE = 0 << PE1;
    // PORTA = 0;

    volatile char *ext_ram = (char *) 0x1CFF;  // Start address for the SRAM
 
    //uint8_t testValue = 'c';
    //while (1) {

        //*ext_ram = 0;
        
    //     if (uart_read() == 'a') {
    //         PORTA = (1 << PA0);
    //     }
    //     uart_send(testValue);
    //     if (uart_read() == 'a') {
    //         PORTA = (0 << PA0);
    //     }
    //     uart_send(testValue);

    //     for (int i = 0; i < 25000; i++) {
    //     }
        
    //     //PORTA = (0 << PA0);

    //     //for (int i = 0; i < 25000; i++) {
    //     //}
    //}

    return 0;
}

void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x1400;  // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test...\n");

    // rand() stores some internal state, so calling this function in a loop
    // will yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();

    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                   i, retrieved_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed); // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                   i, retrieved_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n",
           write_errors, retrieval_errors);
}
