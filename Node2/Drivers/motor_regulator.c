#include "motor_regulator.h"
#include "sam.h"

void motor_regulator_init() {
    pwm_init();

    // Enabling PIOB Pin 23 for phase/dir control and TC2 for encoder
    PMC->PMC_PCER0 = PMC_PCER0_PID13;
    PMC->PMC_PCER0 = PMC_PCER0_PID29;
    // PMC->PMC_PCER1 = 1 << (ID_TC6 - 32);

    // Making Pin 23 an ouput
    PIOC->PIO_PER = PIO_PER_P23;
    PIOC->PIO_OER = PIO_OER_P23;

    // PIOC->PIO_SODR = PIO_SODR_P23; // To set high and low on Dir/Phase
    // PIOC->PIO_CODR = PIO_SODR_P23;

    PIOC->PIO_CODR = PIO_SODR_P23;

    // Configuring TC2 for encoder
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;
    PIOC->PIO_ABSR |= PIO_ABSR_P26;
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    // REG_TC2_CCR0 = 0b101;
    // REG_TC2_CMR0 = 5;
    // REG_TC2_BMR = (1<<8) | (1<<9);
}

void update_motor_pos(pad_t* pad) {
    update_duty_cycle_motor(pad->X);
    if (pad->X < 127) {
        PIOC->PIO_CODR = PIO_SODR_P23;
    } else if (pad->X > 127) {
        PIOC->PIO_SODR = PIO_SODR_P23;
    }
}

void read_encoder(motor_position_t* mot_pos) {
    mot_pos->pos = TC2->TC_CHANNEL[0].TC_CV;
    printf("Encoder value: %ld\n", mot_pos->pos);
}