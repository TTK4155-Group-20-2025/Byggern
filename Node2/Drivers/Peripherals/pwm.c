#include "sam.h"
#include "pwm.h"
#include <math.h>

#define CRPD 0x3345

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
    PWM->PWM_CH_NUM[1].PWM_CMR |= (0 << PWM_CMR_CALG) | (0 << PWM_CMR_CPOL) | PWM_CMR_CPRE_MCK_DIV_128;
    PWM->PWM_CH_NUM[1].PWM_CPRD = CRPD;
    PWM->PWM_CH_NUM[1].PWM_CDTY = (uint32_t)round(degrees_to_cdty(90));
    PWM->PWM_ENA |= PWM_ENA_CHID1;

    PWM->PWM_CH_NUM[0].PWM_CMR |= (0 << PWM_CMR_CALG) | (0 << PWM_CMR_CPOL) | PWM_CMR_CPRE_MCK_DIV_128;
    PWM->PWM_CH_NUM[0].PWM_CPRD = CRPD;
    PWM->PWM_CH_NUM[0].PWM_CDTY = (uint32_t)pad_to_cdty(127);
    PWM->PWM_ENA |= PWM_ENA_CHID0;
}
void update_duty_cycle_servo(int32_t degrees) {
    if (degrees > -90 && degrees < 90) {
        degrees += 90;
        PWM->PWM_CH_NUM[1].PWM_CDTYUPD = (uint32_t)round(degrees_to_cdty(degrees));
    }
}
void update_duty_cycle_motor(int16_t padX) {
    if (padX >= 0 && padX <= 255) {
        PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (uint32_t)round(pad_to_cdty(padX));
    } else {
        PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (uint32_t)round(pad_to_cdty(127));
    }
}
int32_t degrees_to_cdty(int32_t degrees) {
    float temp = (float)degrees;
    temp = -(131.0/36.0)*(temp - 180.0) + 11813.0;
    return (int32_t)temp;
}
int32_t pad_to_cdty(int16_t padX) {
    // printf("temp: %ld\n", padX);
    int32_t temp = abs(padX - 127);
    temp = -(4125.0/127.0)*(temp) + 13125;
    // printf("duty cycle: %ld\n", temp);
    return (int32_t)temp;
}