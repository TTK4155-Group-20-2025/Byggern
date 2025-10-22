/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>

typedef struct can_message_t
{
	ID_t id;
	uint8_t data_length;
	uint8_t data[8];
} CAN_MESSAGE;

typedef enum {
    NODE1 = 0b00,
    NODE2 = 0b10
    // ADD MORE LATER WHEN IMPLEMENTING NDOE 2. DIFFERENT IDs FOR DIFFERENT INSTRUCTIONS
} ID_t;

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

#endif /* CAN_CONTROLLER_H_ */