#include "joystick.h"

uint8_t can_received_joystick_flag = 0; // For interrupts

void joy_pos_read(joystick_pos* pos, CAN_MESSAGE* message) {
    if (can_receive(message, 1) == 0) {    
        pos->X = (int16_t)(message->data[0] << 8);
        pos->X |= (int16_t)(message->data[1] & 0xFF);
        pos->Y = (int16_t)(message->data[2] << 8);
        pos->Y |= (int16_t)(message->data[3] & 0xFF);
    }
}