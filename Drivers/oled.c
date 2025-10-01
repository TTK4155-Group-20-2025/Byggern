#include "oled.h"
#include "fonts.h"

void oled_init() {
    enable_slave_OLED();

    DDRB |= (1 << PB0);
    PORTB &= ~(1 << PB0);

    spi_master_transmit(0xae); // display off
    
    spi_master_transmit(0xa1); // segment remap
    
    // spi_master_transmit(0xda); // common pins hardware config
    // spi_master_transmit(0x12); // A4 set 1 and A5 set 0

    spi_master_transmit(0xc8); // output scan direction
    
    // spi_master_transmit(0xa8); // multiplex ratio 63
    // spi_master_transmit(0x3f);

    // spi_master_transmit(0xd5); // display divide ratio/osc. freq. mode
    // spi_master_transmit(0x80); // default

    // spi_master_transmit(0x81); // Contrast control
    // spi_master_transmit(0x50); // 50%

    // spi_master_transmit(0xd9); // Pre-charge period
    // spi_master_transmit(0x21);

    // spi_master_transmit(0xdb); // VCOM deselect level
    // spi_master_transmit(0x34);
    
    spi_master_transmit(0xa4); // Out follows RAM content
    
    spi_master_transmit(0xa6); // Set normal display
    
    spi_master_transmit(0xaf); // Display ON
    
    oled_clean();

    spi_master_transmit(0x20); // Set Memory Addressing Mode
    spi_master_transmit(0x02);

    disable_all_slaves();
}

void oled_test() {
    enable_slave_OLED();

    PORTB &= ~(1 << PB0);
    spi_master_transmit(0xB4);
    spi_master_transmit(0x00);
    spi_master_transmit(0x10);

    PORTB |= (1 << PB0);

    for (int i = 0; i < 128; i++) {
        spi_master_transmit(0xFF);
    }

    disable_all_slaves();
}

void oled_clean() {
    PORTB &= ~(1 << PB0);

    spi_master_transmit(0x20);
    spi_master_transmit(0x00);

    spi_master_transmit(0x21);
    spi_master_transmit(0x00);
    spi_master_transmit(0xFF);

    spi_master_transmit(0x22);
    spi_master_transmit(0x00);
    spi_master_transmit(7);

    PORTB |= (1 << PB0);

    for (int i = 0; i < 128*8; i++) {
        spi_master_transmit(0x00);
    }

    PORTB &= ~(1 << PB0);
}

void oled_goto_page(uint8_t page) {
    if (page < 8) {
        // uint8_t temp = 0xB0 | page;
        // PORTB &= ~(1 << PB0);
        // spi_master_transmit(temp);

        OLED_DISPLAY->PAGE = page;
    }
}
void oled_goto_column(uint8_t column) {
    if (column < 128) {
        // uint8_t temp_lower = 0x00 | (column & 0b1111);
        // uint8_t temp_upper = 0x10 | ((column >> 4) & 0b1111);
        // PORTB &= ~(1 << PB0);
        // spi_master_transmit(temp_lower);
        // spi_master_transmit(temp_upper);

        OLED_DISPLAY->COL = column;
    }
}
void oled_clean_page(uint8_t page) {
    if (page < 8) {
        oled_goto_page(page);
        oled_goto_column(0);
        PORTB |= (1 << PB0);
        for (int i = 0; i < 128; i++) {
            spi_master_transmit(0x00);
        }
    }
}
void oled_pos(uint8_t page, uint8_t column) {
    oled_goto_column(column);
    oled_goto_page(page);
}
void oled_print_char(char letter) {
    uint8_t byte = 0;
    PORTB |= (1 << PB0);
    for (int i = 0; i < 5; i++) {
        byte = pgm_read_byte(&(font5[(uint8_t)letter - 32][i]));
        spi_master_transmit(byte);
    }
    spi_master_transmit(0x00);
}
void oled_print(char* string) {
    for (int i = 0; string[i] != '\0'; i++) {
        oled_print_char(string[i]);
    }
}
void oled_print_box(uint8_t page) {
    enable_slave_OLED();
    
    //Printing left side
    oled_pos(page, 20);
    PORTB |= (1 << PB0);
    spi_master_transmit(0b11111111);

    //Printing bottom line
    oled_pos(page + 1, 20);
    PORTB |= (1 << PB0);
    for (int i = 20; i < (128-20); i++) {
        spi_master_transmit(0b00000001);
    }

    //Printing upper line
    oled_pos(page - 1, 20);
    PORTB |= (1 << PB0);
    for (int i = 20; i < (128-20); i++) {
        spi_master_transmit(0b10000000);
    }

    //Printing Right side
    oled_pos(page, 127-20);
    PORTB |= (1 << PB0);
    spi_master_transmit(0b11111111);

    disable_all_slaves();
}
void oled_home() {
    enable_slave_OLED();

    char* option_1 = "Play game";
    char* option_2 = "Score";
    oled_pos(3, 40);
    oled_print(option_1);
    oled_pos(5, 50);
    oled_print(option_2);

    disable_all_slaves();
}