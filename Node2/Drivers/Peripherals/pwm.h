

void pwm_init();
void update_duty_cycle_servo(int32_t degrees);
void update_duty_cycle_motor(int32_t u, int32_t start_limit, int32_t end_limit);
int32_t degrees_to_cdty(int32_t degrees);
int32_t pad_to_cdty(int32_t u, int32_t start_limit, int32_t end_limit);