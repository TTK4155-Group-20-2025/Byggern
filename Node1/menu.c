#include "menu.h"

volatile uint8_t button_last_pressed = 0;

// SETTINGMENU???

void menu_init(menu_t* menu) {
    menu->state = HOME;
    oled_home();

    menu->scoreboard[0] = 0;
    menu->scoreboard[1] = 0;
    menu->scoreboard[2] = 0;
    
    menu->select_pos = 3;
}

void play_game(menu_t* menu) {
    menu->state = GAME;
    menu->select_pos = -1;
    oled_play_game(menu->current_score);
} // IKKE ferdig
void enter_score_screen(menu_t* menu) {
    menu->state = SCORE;
    menu->select_pos = 6;
    oled_score(menu->scoreboard);
}
void enter_home_screen(menu_t* menu) {
    menu->state = HOME;
    oled_home();
    menu->select_pos = 3;
}
uint8_t button_pressed() {
    enable_slave_IO_BOARD();
    PORTB &= ~(1 << PB0);
    //printf("PORTB: %u\n", PORTB);
    spi_master_transmit(0x03);
    _delay_us(40);
    uint8_t trash = spi_master_receive();
    _delay_us(2);
    trash = spi_master_receive();
    _delay_us(2);
    uint8_t button = spi_master_receive();
    disable_all_slaves();
    //printf("PORTB after: %u\n", PORTB);
    printf("Is pressed: %u\n", button); // FJERN
    if (button != button_last_pressed && button > 0) {
        button_last_pressed = button;
        return 1;
    }
    button_last_pressed = button;
    return 0;
}
void update_selector(menu_t* menu, position_t* position_joystick) {
    if (menu->state == HOME) {
        if (position_joystick->DIR == DIR_DOWN && menu->select_pos == 3) {
            oled_clear_box(3);
            menu->select_pos = 5;
            oled_print_box(5);
        } else if (position_joystick->DIR == DIR_UP && menu->select_pos == 5) {
            oled_clear_box(5);
            menu->select_pos = 3;
            oled_print_box(3);
        }
    } else if (menu->state == SCORE) {
        // Can add content later
    } else if (menu->state == GAME) {
        // Wait till we know more about the game
    }
}
void update_menu(menu_t* menu, position_t* position_joystick, message_t* message) {
    adc_read_TC(0); // DO NOT REMOVE
    pos_read(position_joystick);
    if (menu->state == GAME) {
        printf("Hei\n"); // FJERN
        // play_game(menu->current_score);
        can_send_joystick(message, position_joystick);
        can_receive(message);
        menu->current_score += get_score();
        printf("Current score: %u", menu->current_score);
        if (message->ID == GAME_END) {
            if (menu->current_score > menu->scoreboard[0]) {
                menu->scoreboard[2] = menu->scoreboard[1];
                menu->scoreboard[1] = menu->scoreboard[0];
                menu->scoreboard[0] = menu->current_score;
            } else if (menu->current_score > menu->scoreboard[1]) {
                menu->scoreboard[2] = menu->scoreboard[2];
                menu->scoreboard[1] = menu->current_score;
            } else if (menu->current_score > menu->scoreboard[2]) {
                menu->scoreboard[2] = menu->current_score;
            }
            enter_score_screen(menu);
            message->ID = JOYSTICK;
        }
    } else {
        direction_read(position_joystick);

        update_selector(menu, position_joystick);

        if (menu->state == HOME && menu->select_pos == 3 && button_pressed()) {
            menu->current_score = 0;
            play_game(menu);
        } else if (menu->state == HOME && menu->select_pos == 5 && button_pressed()) {
            enter_score_screen(menu);
        } else if (menu->state == SCORE && menu->select_pos == 6 && button_pressed()) {
            enter_home_screen(menu);
        }
    }
}