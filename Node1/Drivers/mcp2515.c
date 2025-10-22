#include "mcp2515.h"

uint8_t mcp2515_init() {
    uint8_t value;
    mcp2515_reset();

    //Self test
    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n");
        mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_CONFIG);
    }
    return 0;
}
uint8_t mcp2515_read(uint8_t address_byte) {
    enable_slave_CAN();
    spi_master_transmit(MCP_READ);
    spi_master_transmit(address_byte);
    uint8_t data_byte = spi_master_receive();
    disable_all_slaves();
    return data_byte;
}
void mcp2515_write(uint8_t address_byte, uint8_t data_byte) {
    enable_slave_CAN();
    spi_master_transmit(MCP_WRITE);
    spi_master_transmit(address_byte);
    spi_master_transmit(data_byte);
    disable_all_slaves();
}
uint8_t mcp2515_read_status() {
    enable_slave_CAN();
    spi_master_transmit(MCP_READ_STATUS);
    uint8_t data_byte = spi_master_receive();
    data_byte = spi_master_receive();
    disable_all_slaves();
    return data_byte;
}
void mcp2515_rts(RTS_BUFFER buffer) {
    enable_slave_CAN();
    if (buffer == ALL) {
        spi_master_transmit(MCP_RTS_ALL);
    } else if (buffer == TX0) {
        spi_master_transmit(MCP_RTS_TX0);
    } else if (buffer == TX1) {
        spi_master_transmit(MCP_RTS_TX1);
    } else if (buffer == TX2) {
        spi_master_transmit(MCP_RTS_TX2);
    }
    disable_all_slaves();
}
void mcp2515_bit_modify(uint8_t address_byte, uint8_t mask_byte, uint8_t data_byte) {
    enable_slave_CAN();
    spi_master_transmit(MCP_BITMOD);
    spi_master_transmit(address_byte);
    spi_master_transmit(mask_byte);
    spi_master_transmit(data_byte);
    disable_all_slaves();
}
void mcp2515_reset() {
    enable_slave_CAN();
    spi_master_transmit(MCP_RESET);
    disable_all_slaves();
}