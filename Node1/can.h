#pragma once

#include "Drivers/mcp2515.h"
#include "Drivers/Com/uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Drivers/adc.h"

typedef enum {
    GAME_END = 0b00,
    TURN_ON = 0b01,
    JOYSTICK = 0b10,
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

void can_send_joystick_and_pad(message_t* message, position_t* joy_pos, pad_t* pad, uint8_t button);
void can_send_game_on(message_t* message);