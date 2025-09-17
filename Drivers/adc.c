#include "adc.h"
#include <util/delay.h>

volatile uint8_t *adc_output = (uint8_t *) 0x1234;
volatile uint8_t X_pad = 0;
volatile uint8_t Y_pad = 0;
volatile uint8_t X_joystick = 0;
volatile uint8_t Y_joystick = 0;

void adc_init() {
    DDRD |= (1 << PD5);
    
    // Configuring for generating  a waveform output in CTC mode.
    OCR1A = 0x0000; //top
    TCCR1A |= (0 << COM1A1) | (1 << COM1A0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B |= (1 << WGM12) | (0 << WGM13)| (0 << CS12) | (0 << CS11) | (1 << CS10);

    // Enable timer/counter0 interrupt and start timer
    TCCR0 |= (1 << WGM01) | (1 << CS00);
    TIMSK |= (1 << OCIE0);
    OCR0 = 144;
    adc_output[0] = 0;
}

uint8_t adc_read(uint8_t channel) {
    if (channel == 0) {
        return X_joystick;
    } else if (channel == 1) {
        return Y_joystick;
    } else if (channel == 2) {
        return Y_pad; // Wired to X_pad but pad is inverted.
    } else if (channel == 3) {
        return X_pad; // Wired to Y_pad but pad is inverted.
    } else {
        printf("Invalid channel.\n");
    } return 0;
}

uint8_t testRead(uint8_t channel) {
    if (channel == 0) {
        adc_output[0] = 0;
        _delay_us(100000);
        uint8_t a = adc_output[0];
        return a;
    } else if (channel == 1) {
        return Y_joystick;
    } else if (channel == 2) {
        return Y_pad; // Wired to X_pad but pad is inverted.
    } else if (channel == 3) {
        return X_pad; // Wired to Y_pad but pad is inverted.
    } else {
        printf("Invalid channel.\n");
    } return 0;
}

void pos_calibrate(position_t* position_joystick) {
    uint8_t sum_x = 0;
    uint8_t sum_y = 0;
    uint8_t N = 32;

    for (uint8_t i = 0; i < N; i++) {
        sum_x += adc_read(ADC_CHANNEL_JOYSTICK_X);
        sum_y += adc_read(ADC_CHANNEL_JOYSTICK_Y);
        _delay_us(200);
    }

    position_joystick->initX = (uint8_t)(sum_x/N);
    position_joystick->initY = (uint8_t)(sum_y/N);

    position_joystick->X = position_joystick->initX;
    position_joystick->Y = position_joystick->initY;
}

void pos_read(position_t* position_joystick) {

    int8_t x_pos = adc_read(ADC_CHANNEL_JOYSTICK_X);
    int8_t y_pos = adc_read(ADC_CHANNEL_JOYSTICK_Y);

    int8_t gain_x = (x_pos - position_joystick->initX);
    int8_t gain_y = (y_pos - position_joystick->initY);

    if (gain_x > -DEADZONE && gain_y < DEADZONE) {gain_x = 0;}
    if (gain_y > -DEADZONE && gain_y < DEADZONE) {gain_y = 0;}

    int8_t span_x = 80;
    int8_t span_y = 80;

    int8_t normalized_x = (gain_x * 127) / span_x;
    int8_t normalized_y = (gain_y * 127) / span_y;

    if (normalized_x < -127) {normalized_x = -127;}
    if (normalized_x > 127) {normalized_x = -127;}
    if (normalized_y < -127) {normalized_y = -127;}
    if (normalized_y > 127) {normalized_y = -127;}

    position_joystick->X = normalized_x;
    position_joystick->Y = normalized_y;

    // Values:
    // V_x0 = 2.430;
    // V_y0 = 2.461;

    // V_xmax = 3.865;
    // V_ymax = 3.91;

    // V_xmin = 1.105;
    // V_ymin = 1.02;
}

ISR(TIMER0_COMP_vect) {
    X_joystick = adc_output[0];
    Y_joystick = adc_output[0];
    Y_pad = adc_output[0];
    X_pad = adc_output[0];
    //printf("Hei");

    adc_output[0] = 0;
    //TCNT0 = 0;
}