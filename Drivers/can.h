#include "mcp2515.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {
    NODE1 = 0b00,
    NODE2 = 0b10
    // ADD MORE LATER WHEN IMPLEMENTING NDOE 2. DIFFERENT IDs FOR DIFFERENT INSTRUCTIONS
} ID_t;

typedef struct {
    ID_t ID;
    uint8_t data[8];
    uint8_t datalength;
    RTS_BUFFER buffer;
} message_t;

void can_init();
void can_transmit(message_t* message);
void can_receive(message_t* message);