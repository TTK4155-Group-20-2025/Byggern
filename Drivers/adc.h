#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#define ADC_CHANNEL_JOYSTICK_X 0
#define ADC_CHANNEL_JOYSTICK_Y 1
#define SAMPLES 8
#define DEADZONE 6

void adc_init();
uint8_t adc_read(uint8_t channel);
uint8_t testRead(uint8_t channel);

// typedef enum {
//     LEFT = 0,
//     RIGHT = 1,
//     UP = 2,
//     DOWN = 3,
//     NEUTRAL = 4
// } direction;


typedef struct {
    int8_t X;
    int8_t Y;

    int8_t initX;
    int8_t initY;
} position_t;