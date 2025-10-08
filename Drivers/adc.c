#include "adc.h"

#define ADC_CHANNEL_JOYSTICK_X 0
#define ADC_CHANNEL_JOYSTICK_Y 1

volatile uint8_t *adc_output = (uint8_t *) 0x1234;
volatile int16_t X_pads = 0;
volatile int16_t Y_pads = 0;
volatile int16_t X_joystick = 0;
volatile int16_t Y_joystick = 0;
volatile uint8_t read_pos_flag = 0;

void adc_init() {
    DDRD |= (1 << PD5);
    
    // Configuring for generating  a waveform output in CTC mode.
    OCR1A = 0x0000; //top
    TCCR1A |= (0 << COM1A1) | (1 << COM1A0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B |= (1 << WGM12) | (0 << WGM13)| (0 << CS12) | (0 << CS11) | (1 << CS10);

    //Start converting
    //adc_output[0] = 0;
    //_delay_us(40);

    // Enable timer/counter0 interrupt and start timer
    // TCCR0 |= (1 << WGM01) | (1 << CS02) | (0 << CS01) | (1 << CS00);
    // TIMSK |= (1 << OCIE0);
    // OCR0 = 48;
    TCCR3A |= (1 << COM3A1);
    TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
    ETIMSK |= (1 << OCIE3A);
    OCR3A = 48;
}

// Reading ADC with timer/counter0
int16_t adc_read_TC(uint8_t channel) {
    if (read_pos_flag) {
        X_joystick = adc_output[0];
        Y_joystick = adc_output[0];
        Y_pads = adc_output[0];
        X_pads = adc_output[0];

        adc_output[0] = 0;

        if (channel == 0) {
            return X_joystick;
        } else if (channel == 1) {
            return Y_joystick;
        } else if (channel == 2) {
            return Y_pads; // Wired to X_pad but pad is inverted.
        } else if (channel == 3) {
            return X_pads; // Wired to Y_pad but pad is inverted.
        } else {
            printf("Invalid channel.\n");
        } return 0;

        read_pos_flag = 0;
    }
}

// Reading ADC with delay (USED FOR CALIBRATION)
int16_t adc_read(uint8_t channel) {
    adc_output[0] = 0;
    _delay_us(40);
    int16_t X_joy = adc_output[0];
    int16_t Y_joy = adc_output[0];
    int16_t Y_pad = adc_output[0];
    int16_t X_pad = adc_output[0];

    if (channel == 0) {
        return X_joy;
    } else if (channel == 1) {
        return Y_joy;
    } else if (channel == 2) {
        return Y_pad; // Wired to X_pad but pad is inverted.
    } else if (channel == 3) {
        return X_pad; // Wired to Y_pad but pad is inverted.
    } else {
        printf("Invalid channel.\n");
    } return 0;
}

void pos_calibrate(position_t* position_joystick) {
    int16_t sum_x = 0;
    int16_t sum_y = 0;
    int16_t N = 32;

    for (int16_t i = 0; i < N; i++) {
        sum_x += adc_read(ADC_CHANNEL_JOYSTICK_X);
        sum_y += adc_read(ADC_CHANNEL_JOYSTICK_Y);
    }

    position_joystick->initX = (int16_t)(sum_x/N);
    position_joystick->initY = (int16_t)(sum_y/N);
    position_joystick->X = 0;
    position_joystick->Y = 0;

    //Start ADC converting
    adc_output[0] = 0;
    _delay_us(40);
}

void pos_read(position_t* position_joystick) {
    int16_t x_pos_read = X_joystick;
    int16_t y_pos_read = Y_joystick;

    int16_t y_percent = 0;
    int16_t x_percent = 0;

    int16_t max = 245;
    int16_t min = 75;

    if (x_pos_read > position_joystick->initX) {
        x_percent = (100*(x_pos_read - position_joystick->initX))/(max - position_joystick->initX);
        if (x_percent > 100) {
            x_percent = 100;
        }
    } else {
        x_percent = -(100*(x_pos_read - position_joystick->initX))/(min - position_joystick->initX);
        if (x_percent < -100) {
            x_percent = -100;
        }
    }
    if (y_pos_read > position_joystick->initY) {
        y_percent = (100*(y_pos_read - position_joystick->initY))/(max - position_joystick->initY);
        if (y_percent > 100) {
            y_percent = 100;
        }
    } else {
        y_percent = -(100*(y_pos_read - position_joystick->initY))/(min - position_joystick->initY);
        if (y_percent < -100) {
            y_percent = -100;
        }
    }

    // FOR TESTING:
    position_joystick->X = x_percent;
    //printf("X_precent: %" PRIi16 "\n", position_joystick->X);
    position_joystick->Y = y_percent;
    //printf("Y_precent: %" PRIi16 "\n", position_joystick->Y);
}

void direction_read(position_t* position_joystick) {
    // int8_t neutral_zone = 20;

    if (position_joystick->Y > 40 && position_joystick->Y >= abs(position_joystick->X)) {
        position_joystick->DIR = DIR_UP;
        // printf("Up\n");
    } else if (position_joystick->Y < -40 && abs(position_joystick->Y) >= abs(position_joystick->X)) {
        position_joystick->DIR = DIR_DOWN;
        // printf("Down\n");
    } else if (position_joystick->X > 40 && abs(position_joystick->X) > abs(position_joystick->Y)) {
        position_joystick->DIR = DIR_RIGHT;
        // printf("Right\n");
    } else if (position_joystick->X < -40 && abs(position_joystick->X) < abs(position_joystick->Y)) {
        position_joystick->DIR = DIR_LEFT;
        // printf("Left\n");
    } else {
        position_joystick->DIR = DIR_NEUTRAL;
        // printf("Neutral\n");
    }
}

ISR(TIMER3_COMPA_vect) {
    read_pos_flag = 1;
}