#include "oled.h"
#include "fonts.h"

#define COLS 128
#define PAGES 8

volatile uint8_t update_oled_flag = 0;
volatile uint8_t* OLED_DISPLAY = (volatile uint8_t*)0x1600;

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
    
    // Clean screen on init
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

    // Set Memory Addressing Mode
    spi_master_transmit(0x20); // Set Memory Addressing Mode
    spi_master_transmit(0x02);

    disable_all_slaves();

    oled_clean();

    // Enable timer/counter3B for updating oled
    TCCR3A |= (1 << COM3B1);
    TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
    ETIMSK |= (1 << OCIE3B);
    OCR3B = 48;
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
    for (int i = 0; i < 8; i++) {
        oled_clean_page(i);
    }
}

void oled_goto_page(uint8_t page) {
    if (page < 8) {
        uint8_t temp = 0xB0 | page;
        PORTB &= ~(1 << PB0);
        spi_master_transmit(temp);
    }
}
void oled_goto_column(uint8_t column) {
    if (column < 128) {
        uint8_t temp_lower = 0x00 | (column & 0b1111);
        uint8_t temp_upper = 0x10 | ((column >> 4) & 0b1111);
        PORTB &= ~(1 << PB0);
        spi_master_transmit(temp_lower);
        spi_master_transmit(temp_upper);
    }
}
void oled_clean_page(uint8_t page) {
    if (page < 8) {
        // oled_goto_page(page);
        // oled_goto_column(0);
        // PORTB |= (1 << PB0);
        for (int i = 0; i < 128; i++) {
            // spi_master_transmit(0x00);
            OLED_DISPLAY[page*128 + i] = 0b00000000;
        }
    }
}
void oled_pos(uint8_t page, uint8_t column) {
    oled_goto_column(column);
    oled_goto_page(page);
}
void oled_print_char(char letter, uint8_t page, uint8_t column) {
    uint8_t byte = 0;
    // PORTB |= (1 << PB0);
    // for (int i = 0; i < 5; i++) {
    //     byte = pgm_read_byte(&(font5[(uint8_t)letter - 32][i]));
    //     spi_master_transmit(byte);
    // }
    // spi_master_transmit(0x00);
    for (int i = 0; i < 5; i++) {
        byte = pgm_read_byte(&(font5[(uint8_t)letter - 32][i]));
        OLED_DISPLAY[page*128 + i+column] = byte;
    } OLED_DISPLAY[page*128 + column + 5] = 0b00000000;
}
void oled_print(char* string, uint8_t page, uint8_t column) {

    for (int i = 0; string[i] != '\0'; i++) {
        oled_print_char(string[i], page, column);
        column += 6;
    }
}
void oled_print_box(uint8_t page) {
    // enable_slave_OLED();
    
    // //Printing left side
    // oled_pos(page, 20);
    // PORTB |= (1 << PB0);
    // spi_master_transmit(0b11111111);

    // //Printing bottom line
    // oled_pos(page + 1, 20);
    // PORTB |= (1 << PB0);
    // for (int i = 20; i < (128-20); i++) {
    //     spi_master_transmit(0b00000001);
    // }

    // //Printing upper line
    // oled_pos(page - 1, 20);
    // PORTB |= (1 << PB0);
    // for (int i = 20; i < (128-20); i++) {
    //     spi_master_transmit(0b10000000);
    // }

    // //Printing Right side
    // oled_pos(page, 127-20);
    // PORTB |= (1 << PB0);
    // spi_master_transmit(0b11111111);

    // disable_all_slaves();
    OLED_DISPLAY[page*128 + 20] = 0b11111111;
    for (int i = 20; i < (128-20); i++) {
        OLED_DISPLAY[(page + 1)*128 + i] = 0b00000001;
    } for (int i = 20; i < (128-20); i++) {
        OLED_DISPLAY[(page - 1)*128 + i] = 0b10000000;
    } OLED_DISPLAY[page*128 + 127-20] = 0b11111111;
}
void oled_clear_box(uint8_t page) {
    OLED_DISPLAY[page*128 + 20] = 0b00000000;
    for (int i = 20; i < (128-20); i++) {
        OLED_DISPLAY[(page + 1)*128 + i] = 0b00000000;
    } for (int i = 20; i < (128-20); i++) {
        OLED_DISPLAY[(page - 1)*128 + i] = 0b00000000;
    } OLED_DISPLAY[page*128 + 127-20] = 0b00000000;
}
void oled_home() {
    oled_clean();

    char* option_1 = "Play game";
    char* option_2 = "Score";

    oled_print(option_1, 3, 40);
    oled_print(option_2, 5, 50);
    oled_print_box(3);
}
void oled_score(uint16_t* scoreboard) {
    oled_clean();
    char* option_1 = "Back";
    char* title = "Scores";
    oled_print(title, 0, 40);
    oled_print(option_1, 6, 48);

    oled_print_box(6);

    uint8_t byte = 0;
    for (int i = 0; i < 3; i++) {
        oled_print_char((char)(49 + i), 2+i, 40);
        // oled_print(scoreboard[i], 2 + i, 50);
    }

    uint16_t temp = scoreboard[0];
    uint16_t temp2 = scoreboard[0];
    temp = temp / 100;
    oled_print_char(temp + 48, 2, 50);
    temp2 = scoreboard[0] - temp*100;
    temp2 = temp2 / 10;
    oled_print_char(temp2 + 48, 2, 56);
    temp = scoreboard[0] - temp*100 - temp2*10;
    oled_print_char(temp + 48, 2, 62);

    temp = scoreboard[1];
    temp2 = scoreboard[1];
    temp = temp / 100;
    oled_print_char(temp + 48, 3, 50);
    temp2 = scoreboard[1] - temp*100;
    temp2 = temp2 / 10;
    oled_print_char(temp2 + 48, 3, 56);
    temp = scoreboard[1] - temp*100 - temp2*10;
    oled_print_char(temp + 48, 3, 62);

    temp = scoreboard[2];
    temp2 = scoreboard[2];
    temp = temp / 100;
    oled_print_char(temp + 48, 4, 50);
    temp2 = scoreboard[2] - temp*100;
    temp2 = temp2 / 10;
    oled_print_char(temp2 + 48, 4, 56);
    temp = scoreboard[2] - temp*100 - temp2*10;
    oled_print_char(temp + 48, 4, 62);
}
void oled_play_game(uint16_t score) {
    oled_clean();
    
    // char* title = "Score:";
    // oled_print(title, 2, 50);

    // uint16_t temp = score;
    // uint16_t temp2 = score;
    // temp = temp / 100;
    // oled_print_char(temp + 48, 3, 45);
    // temp2 = score - temp*100;
    // temp2 = temp2 / 10;
    // oled_print_char(temp2 + 48, 3, 55);
    // temp = score - temp*100 - temp2*10;
    // oled_print_char(temp + 48, 3, 65);
    char* title = "The game has started";
    char* motivation = "You can do this!";
    oled_print(title, 3, 5);
    oled_print(motivation, 5, 20);
}

void oled_update() {
    if (update_oled_flag) {
        enable_slave_OLED();
        //printf("PORTB: %u\n", PORTB);
        for (int i = 0; i < 8; i++) {
            oled_goto_page(i);
            oled_goto_column(0);
            PORTB |= (1 << PB0);
            for (int j = 0; j < 128; j++) {
                spi_master_transmit(OLED_DISPLAY[i*128 + j]);
            }
        }
        update_oled_flag = 0;
        disable_all_slaves();
        //printf("PORTB after: %u\n", PORTB);
    }
}

ISR(TIMER3_COMPB_vect) {
    update_oled_flag = 1;
}