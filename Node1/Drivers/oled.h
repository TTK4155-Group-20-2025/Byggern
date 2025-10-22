#include "Com/spi.h"
#include "Com/uart.h"
#include <stdio.h>

void oled_init();
void oled_clean();
void oled_test();

void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);
void oled_clean_page(uint8_t page);
void oled_pos(uint8_t page, uint8_t column);

void oled_print_char(char letter, uint8_t page, uint8_t column);
void oled_print(char* string, uint8_t page, uint8_t column);
void oled_print_box(uint8_t page);
void oled_clear_box(uint8_t page);
void oled_home();
void oled_score(char** scoreboard);

void oled_update();