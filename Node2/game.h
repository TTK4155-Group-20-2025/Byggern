#pragma once
#include "Drivers/photodiode.h"
#include "Drivers/motor_regulator.h"
#include "Drivers/solenoid.h"
#include "Drivers/joystick.h"
#include "CAN_node_2/can_controller.h"

void game_init();
uint8_t has_game_started(CAN_MESSAGE* message);
void game(CAN_MESSAGE* message, motor_position_t* mot_pos, pad_t* pad, joystick_pos* joy_pos);