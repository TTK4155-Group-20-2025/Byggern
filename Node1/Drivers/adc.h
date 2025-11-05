#pragma once

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <math.h>

#define F_CPU 4195200UL

#include <util/delay.h>
#include <inttypes.h>

typedef enum {
    DIR_LEFT = 0,
    DIR_RIGHT = 1,
    DIR_UP = 2,
    DIR_DOWN = 3,
    DIR_NEUTRAL = 4
} direction;

typedef struct {
    int16_t X;
    int16_t Y;

    int16_t initX;
    int16_t initY;

    direction DIR;
} position_t;

typedef struct {
    int16_t X;
    int16_t Y;
} pad_t;

void adc_init();
int16_t adc_read_TC(uint8_t channel);
int16_t adc_read(uint8_t channel);
void pos_calibrate(position_t* position_joystick);
void pos_read(position_t* position_joystick);
void direction_read(position_t* position_joystick);
uint16_t get_score();