#include "joystick.h"
#include "Peripherals/pwm.h"

typedef struct {
    int32_t pos;
} motor_position_t;

void motor_regulator_init();
void update_motor_pos(pad_t* pad);
void read_encoder(motor_position_t* mot_pos);