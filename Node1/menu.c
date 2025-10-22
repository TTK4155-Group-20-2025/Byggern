#include "menu.h"

// SETTINGMENU???

void menu_init(menu_t* menu) {
    menu->state = HOME;
    oled_home();

    menu->scoreboard[0] = "Bård";
    menu->scoreboard[1] = "Amund";
    menu->scoreboard[2] = "Regine";
    
    menu->select_pos = 3;
}

void play_game(menu_t* menu) {

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
    printf("Is pressed: %u\n", button);
    return button;
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
void update_menu(menu_t* menu, position_t* position_joystick) {
    adc_read_TC(0); // DO NOT REMOVE
    pos_read(position_joystick);
    direction_read(position_joystick);

    update_selector(menu, position_joystick);

    if (menu->state == HOME && menu->select_pos == 3 && button_pressed()) {
        //play_game(menu);
    } else if (menu->state == HOME && menu->select_pos == 5 && button_pressed()) {
        enter_score_screen(menu);
    } else if (menu->state == SCORE && menu->select_pos == 6 && button_pressed()) {
        enter_home_screen(menu);
    }
}