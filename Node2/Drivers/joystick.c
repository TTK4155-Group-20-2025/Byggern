#include "joystick.h"
#include <math.h>

uint8_t can_received_joystick_flag = 0; // For interrupts

void joy_pos_read(joystick_pos* pos, CAN_MESSAGE* message) {
    if (can_receive(message, 1) == 0) {
        // Maybe add for loop
        pos->X = (int16_t)(message->data[0] << 8);
        pos->X |= (int16_t)(message->data[1] & 0xFF);
        pos->Y = (int16_t)(message->data[2] << 8);
        pos->Y |= (int16_t)(message->data[3] & 0xFF);

        if (abs(pos->X) < 10 && abs(pos->Y) < 10) {
            pos->degrees = 0;
        } else if (abs(pos->Y) < 10 && pos->X != 0) {
            pos->degrees = -pos->X/abs(pos->X)*90;
        } else {
            float tempX = (float)pos->X;
            float tempY = (float)pos->Y;
            pos->degrees = (int32_t)round(-atan2(tempX, abs(tempY))*180.0/3.14);
        }
    }
}