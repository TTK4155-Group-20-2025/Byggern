#include "sam.h"
#include "game.h"

volatile uint8_t game_on = 0;

void game_init() {
    motor_regulator_init();
    photodiode_init();
    solenoid_init();
}
uint8_t has_game_started(CAN_MESSAGE* message) {
    if (can_receive(message, 2) == 0) {
        return 1;
    } return 0;
}
void game(CAN_MESSAGE* message, motor_position_t* mot_pos, pad_t* pad, joystick_pos* joy_pos) {
    game_on = has_game_started(message);
    while (game_on) {
        joy_pos_read(joy_pos, message, pad);
        update_duty_cycle_servo(joy_pos->degrees);
        game_on = game_end(message);
        solenoid_activate(pad);
        control_motor(mot_pos, pad);
    }
    pad->X = 127;
    joy_pos->X = 0;
    joy_pos->Y = 0;
    joy_pos->degrees = 0;
    control_motor(mot_pos, pad);
    update_duty_cycle_servo(joy_pos->degrees);
}