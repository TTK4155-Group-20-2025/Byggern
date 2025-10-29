#include "Drivers/mcp2515.h"
#include "Drivers/Com/uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Drivers/adc.h"

typedef enum {
    SENSOR = 0b00,
    JOYSTICK = 0b10,
    TURNON = 0b01
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

void can_send_joystick(message_t* message, position_t* joy_pos);