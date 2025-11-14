#pragma once
#include <stdint.h>
#include "../CAN_node_2/can_controller.h"
#include "StarterCode/uart.h"

void photodiode_init();
uint8_t game_end(CAN_MESSAGE* message);

uint32_t photodiode_test();

void ADC_Handler(void);
void TC1_Handler(void);