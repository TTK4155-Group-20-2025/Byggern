#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    int8_t X;
    int8_t Y;

    int8_t initX;
    int8_t initY;
} position_t;

void adc_init();
uint8_t adc_read_TC(uint8_t channel);
uint8_t adc_read(uint8_t channel);
void pos_calibrate(position_t* position_joystick);
void pos_read(position_t* position_joystick);

// typedef enum {
//     LEFT = 0,
//     RIGHT = 1,
//     UP = 2,
//     DOWN = 3,
//     NEUTRAL = 4
// } direction;