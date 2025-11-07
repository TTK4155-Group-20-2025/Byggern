#include "Drivers/oled.h"
#include "Drivers/Com/spi.h"
#include "Drivers/adc.h"
#include "Drivers/Com/uart.h"
#include "can.h"

#define F_CPU 4195200UL

#include <util/delay.h>

// SETTINGMENU???

typedef enum {
    HOME,
    GAME,
    SCORE,
    TEST
} screen_state_t;

typedef struct {
    screen_state_t state;
    uint8_t select_pos;
    uint16_t scoreboard[3];
    uint16_t current_score;
} menu_t;

void menu_init(menu_t* menu);
void play_game(menu_t* menu); // IKKE ferdig
void enter_score_screen(menu_t* menu);
void enter_home_screen(menu_t* menu);
uint8_t button_pressed();
void update_selector(menu_t* menu, position_t* position_joystick);
void update_menu(menu_t* menu, position_t* position_joystick, message_t* message, pad_t* position_pad);