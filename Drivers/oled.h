#include "spi.h"
#include "uart.h"
#include <stdio.h>

#define OLED_DISPLAY ((oled_t*) 0x1500)

#define COLS 128

volatile uint8_t page0[COLS];
volatile uint8_t page1[COLS];
volatile uint8_t page2[COLS];
volatile uint8_t page3[COLS];
volatile uint8_t page4[COLS];
volatile uint8_t page5[COLS];
volatile uint8_t page6[COLS];
volatile uint8_t page7[COLS];

typedef struct{
    volatile uint8_t page0[COLS];
    volatile uint8_t page1[COLS];
    volatile uint8_t page2[COLS];
    volatile uint8_t page3[COLS];
    volatile uint8_t page4[COLS];
    volatile uint8_t page5[COLS];
    volatile uint8_t page6[COLS];
    volatile uint8_t page7[COLS];
    volatile uint8_t PAGE;
    volatile uint8_t COL;
} oled_t;

void oled_init();
void oled_clean();
void oled_test();

void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);
void oled_clean_page(uint8_t page);
void oled_pos(uint8_t page, uint8_t column);

void oled_print_char(char letter);
void oled_print(char* string);
void oled_print_box(uint8_t page);

void oled_home();