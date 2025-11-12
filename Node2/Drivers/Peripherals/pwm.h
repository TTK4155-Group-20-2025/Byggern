

void pwm_init();
void update_duty_cycle_servo(int32_t degrees);
void update_duty_cycle_motor(int32_t u, float start, float end);
int32_t degrees_to_cdty(int32_t degrees);
int32_t pad_to_cdty(int32_t u, float start, float end);