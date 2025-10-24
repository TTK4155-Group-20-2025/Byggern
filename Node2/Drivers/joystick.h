#include <stdint.h>
#include "../CAN_node_2/can_controller.h"

typedef struct {
    int16_t X;
    int16_t Y;
} joystick_pos;

void joy_pos_read(joystick_pos* pos, CAN_MESSAGE* message);