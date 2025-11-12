#include "sam.h"
#include "pwm.h"
#include <math.h>

#define CRPD_SERVO 0x3345
#define CRPD_MOTOR 0x19a28

void pwm_init() {
    // Enable PIOB and PWM PMC controllers
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

    // Disable PB13, PB12 and enable peripheral B
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_PDR |= PIO_PDR_P12;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P12;
    
    // Enable PWM clocks
    PWM->PWM_CH_NUM[1].PWM_CMR = (0 << PWM_CMR_CALG) | (0 << PWM_CMR_CPOL) | PWM_CMR_CPRE_MCK_DIV_128;
    PWM->PWM_CH_NUM[1].PWM_CPRD = CRPD_SERVO;
    PWM->PWM_CH_NUM[1].PWM_CDTY = (uint32_t)round(degrees_to_cdty(90));
    PWM->PWM_ENA |= PWM_ENA_CHID1;

    PWM->PWM_CH_NUM[0].PWM_CMR = (0 << PWM_CMR_CALG) | (0 << PWM_CMR_CPOL) | PWM_CMR_CPRE_MCK_DIV_16;
    PWM->PWM_CH_NUM[0].PWM_CPRD = CRPD_MOTOR;
    PWM->PWM_CH_NUM[0].PWM_CDTY = (uint32_t)pad_to_cdty(0, 0, 100);
    PWM->PWM_ENA |= PWM_ENA_CHID0;
}
void update_duty_cycle_servo(int32_t degrees) {
    if (degrees > -90 && degrees < 90) {
        degrees += 90;
        PWM->PWM_CH_NUM[1].PWM_CDTYUPD = (uint32_t)round(degrees_to_cdty(degrees));
    }
}
void update_duty_cycle_motor(int32_t u, int32_t limit) {
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (uint32_t)pad_to_cdty(u, start_limit, end_limit);
}
int32_t degrees_to_cdty(int32_t degrees) {
    float temp = (float)degrees;
    temp = -(131.0/36.0)*(temp - 180.0) + 11813.0;
    return (int32_t)temp;
}
int32_t motor_input_to_cdty(int32_t u, int32_t limit) {
    int32_t temp = abs(u);
    if (temp > limit) {
        temp = limit;
    }
    temp = ((80000 - 105000)*temp) / limit + 105000;
    printf("duty cycle: %ld\n", temp);
    return (int32_t)temp;
}