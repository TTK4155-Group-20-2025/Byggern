#include "joystick.h"
#include "Peripherals/pwm.h"

typedef struct {
    int32_t pos;
} motor_position_t;

void motor_regulator_init();
void update_motor_pos(int32_t u, int32_t start, int32_t end);
void read_encoder(motor_position_t* mot_pos);
void control_motor(motor_position_t* mot_pos, pad_t* pad);

void TC0_Handler(void);