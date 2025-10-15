#include "can.h"

volatile uint8_t can_received_flag = 1;

void can_init() {
    mcp2515_init();
    //GICR |= (1 << INT1);
    // TCCR0 |= (1 << WGM01) | (1 << CS02) | (0 << CS01) | (1 << CS00);
    // TIMSK |= (1 << OCIE0);
    // OCR0 = 48;

    // Filter and masks for node 1. ID: 0b00000000000
    mcp2515_write(MCP_RXF0SIDH, 0x00);
    mcp2515_write(MCP_RXF0SIDL, 0x00);
    mcp2515_write(MCP_RXF1SIDH, 0x00);
    mcp2515_write(MCP_RXF1SIDL, 0x00);
    mcp2515_write(MCP_RXM0SIDH, 0x00);
    mcp2515_write(MCP_RXM0SIDL, 0b01100000);

    // UPDATE THESE VALUES BELOW WHEN IMPLEMENTING NODE 2
    mcp2515_bit_modify(MCP_CNF1, 0b11000000, SJW1);
    mcp2515_bit_modify(MCP_CNF1, 0b00111111, 0x00);
    mcp2515_bit_modify(MCP_CNF2, 0b11111111, 0x00010001);
    mcp2515_bit_modify(MCP_CNF3, 0b00000000, 0x00000000);

    mcp2515_bit_modify(MCP_CANINTE, 0x00, 0x01);
    mcp2515_bit_modify(MCP_CANINTF, 0x00, 0x01);
    

    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK); // CHANGE WHEN NODE 2 IS BEING IMPLEMENTED
}

void can_transmit(message_t* message) {
    uint16_t id = (uint16_t)message->ID;
    uint8_t id_lower = id << 5;
    id = id >> 8;
    uint8_t id_upper = id;
    if (message->buffer == ALL) {
        while(!(mcp2515_read_status() & 0b01010100));
        
        mcp2515_write(MCP_TXB0SIDH, id_upper);
        mcp2515_write(MCP_TXB0SIDL, id_lower);
        mcp2515_write(MCP_TXB1SIDH, id_upper);
        mcp2515_write(MCP_TXB1SIDL, id_lower);
        mcp2515_write(MCP_TXB2SIDH, id_upper);
        mcp2515_write(MCP_TXB2SIDL, id_lower);

        mcp2515_write(MCP_TXB0DLC, message->datalength);
        mcp2515_write(MCP_TXB1DLC, message->datalength);
        mcp2515_write(MCP_TXB2DLC, message->datalength);

        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
        }
        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB1D0 + i, message->data[i]);
        }
        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB2D0 + i, message->data[i]);
        }

        mcp2515_bit_modify(MCP_CANINTE, 0b0011100, 0x00);

        mcp2515_rts(message->buffer);
    } 
    
    else if (message->buffer == TX0) {
        while(!(mcp2515_read_status() & 0b00000100));
        
        mcp2515_write(MCP_TXB0SIDH, id_upper);
        mcp2515_write(MCP_TXB0SIDL, id_lower);

        mcp2515_write(MCP_TXB0DLC, message->datalength);

        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
        }

        mcp2515_bit_modify(MCP_CANINTE, 0b0000100, 0x00);

        mcp2515_rts(message->buffer);
    } 
    
    else if (message->buffer == TX1) {
        while(!(mcp2515_read_status() & 0b00010000));
        
        mcp2515_write(MCP_TXB1SIDH, id_upper);
        mcp2515_write(MCP_TXB1SIDL, id_lower);

        mcp2515_write(MCP_TXB1DLC, message->datalength);

        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB1D0 + i, message->data[i]);
        }

        mcp2515_bit_modify(MCP_CANINTE, 0b00001000, 0x00);

        mcp2515_rts(message->buffer);
    } 
    
    else if (message->buffer == TX2) {
        while(!(mcp2515_read_status() & 0b01000000));
        
        mcp2515_write(MCP_TXB2SIDH, id_upper);
        mcp2515_write(MCP_TXB2SIDL, id_lower);

        mcp2515_write(MCP_TXB2DLC, message->datalength);

        for (int i = 0; i < message->datalength; i++) {
            mcp2515_write(MCP_TXB2D0 + i, message->data[i]);
        }

        mcp2515_bit_modify(MCP_CANINTE, 0b00010000, 0x00);

        mcp2515_rts(message->buffer);
    }
}

void can_receive(message_t* message) {
    if (can_received_flag) {
        message->ID = (ID_t)(mcp2515_read(MCP_RXB0SIDH) << 8);
        message->ID |= (ID_t)(mcp2515_read(MCP_RXB0SIDL));

        message->datalength = mcp2515_read(MCP_RXB0DLC);

        for (int i = 0; i < message->datalength; i++) {
            message->data[i] = mcp2515_read(MCP_RXB0D0 + i);
        }
        uint8_t temp = message->data[0];

        printf("Recieved: %u\n", message->data[0]);

        can_received_flag = 1;
    }
}

ISR(INT1_vect) {
    can_received_flag = 1;
    mcp2515_bit_modify(MCP_CANINTF, 0x00, 0x01);
}

// ISR(TIMER0_COMP_vect) {

// }