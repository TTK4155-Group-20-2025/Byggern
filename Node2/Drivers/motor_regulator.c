#include "motor_regulator.h"
#include "sam.h"

volatile uint8_t control_motor_flag = 0;
volatile int32_t start_pos = 0;
volatile int32_t end_pos = 0;
volatile int32_t integral = 0;

volatile int32_t K_p = 3;
volatile int32_t K_i = 0;

void motor_regulator_init() {
    pwm_init();

    // Enabling PIOB Pin 23 for phase/dir control and TC2 for encoder
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PMC->PMC_PCER0 |= 1 << ID_TC2;
    PMC->PMC_PCER1 |= 1 << (ID_TC6 - 32);
    PMC->PMC_PCER0 |= 1 << ID_TC0;


    // Making Pin 23 an ouput
    PIOC->PIO_PER = PIO_PER_P23;
    PIOC->PIO_OER = PIO_OER_P23;

    // PIOC->PIO_SODR = PIO_SODR_P23; // To set high and low on Dir/Phase
    // PIOC->PIO_CODR = PIO_SODR_P23;

    PIOC->PIO_SODR = PIO_SODR_P23;

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

    // Moving to start position
    motor_position_t last;
    last.pos = 100000;
    motor_position_t current;
    read_encoder(&current);
    update_duty_cycle_motor(100, 0.0, 127.0);
    for (int i = 0; i < 100000; i++);
    while(last.pos != current.pos) {
        read_encoder(&last);
        for (int i = 0; i < 100000; i++);
        read_encoder(&current);
    }

    // Resetting encoder
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;
    PIOC->PIO_ABSR |= PIO_ABSR_P26;
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN;
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    start_pos = current.pos;
    start_pos += 6; // Add margin

    // Moving to end position
    last.pos = 100000;
    PIOC->PIO_CODR = PIO_SODR_P23;
    for (int i = 0; i < 100000; i++);
    while(last.pos != current.pos) {
        read_encoder(&last);
        for (int i = 0; i < 100000; i++);
        read_encoder(&current);
    } end_pos = current.pos;
    end_pos -= 6; // Add margin
    update_duty_cycle_motor(0, 0.0, 127.0);

    // Configuring TC0 for sample time
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_ACPC_CLEAR | TC_CMR_ASWTRG_SET | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4;
    TC0->TC_CHANNEL[0].TC_RC = 656250;
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void update_motor_pos(int32_t u, float start, float end) {
    update_duty_cycle_motor(u, start, end);
    if (u < 0) {
        PIOC->PIO_SODR = PIO_SODR_P23;
    } else if (u >= 0) {
        PIOC->PIO_CODR = PIO_SODR_P23;
    }
}

void read_encoder(motor_position_t* mot_pos) {
    mot_pos->pos = TC2->TC_CHANNEL[0].TC_CV;
    printf("Encoder value: %ld\n", mot_pos->pos); // FJERN
}

void control_motor(motor_position_t* mot_pos, pad_t* pad) {
    if (control_motor_flag) {
        read_encoder(mot_pos);
        printf("pad: %6ld\n", pad->X);
        int32_t ref_pos = -(end_pos*100 - start_pos*100)/(255*100) * (pad->X - 255) + start_pos;
        printf("ref: %6ld\n", ref_pos);
        int32_t error = 0;
        if (abs((ref_pos - mot_pos->pos) - error) > 8) {
            error = ref_pos - mot_pos->pos;
        }
        printf("error: %6ld\n", error);
        integral += error;

        int32_t u = K_p*error + K_i*integral;
        printf("u: %6ld\n", u);

        update_motor_pos(u, -10000.0, 10000.0);

        control_motor_flag = 0;
    }
}

void TC0_Handler(void) {
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR; // Read status to clear interrupt
    control_motor_flag = 1;
}