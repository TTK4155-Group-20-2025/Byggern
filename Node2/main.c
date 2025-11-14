#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "StarterCode/uart.h"
#include "CAN_node_2/can_controller.h"
#include "Drivers/joystick.h"
#include "Drivers/Peripherals/pwm.h"
#include "Drivers/photodiode.h"
#include "Drivers/motor_regulator.h"
#include "Drivers/solenoid.h"
#include "game.h"

#define F_CPU 84000000
#define BAUDRATE 9600

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */

int main()
{
    SystemInit();
    uart_init(F_CPU, BAUDRATE);
    can_init_def_tx_rx_mb((CAN_BR_BRP_Msk & (0x68 << CAN_BR_BRP_Pos))
     | (CAN_BR_SJW_Msk & (0 << CAN_BR_SJW_Pos)) 
     | (CAN_BR_SMP_ONCE)
     | (CAN_BR_PROPAG_Msk & (1 << CAN_BR_PROPAG_Pos))
     | (CAN_BR_PHASE1_Msk & (2 << CAN_BR_PHASE1_Pos))
     | (CAN_BR_PHASE2_Msk & (2 << CAN_BR_PHASE2_Pos)));
    // motor_regulator_init();
    // photodiode_init();
    // solenoid_init();
    game_init();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // int16_t b = (int16_t)0;
    // int16_t c = (int16_t)0;
    // int32_t d = (int32_t)0;
    // int16_t e = 0;
    CAN_MESSAGE message;
    joystick_pos JoyStick;
    JoyStick.X = 0;
    JoyStick.Y = 0;
    JoyStick.degrees = 0;
    pad_t Pad;
    Pad.button = 0;
    Pad.X = 127;
    motor_position_t Mot_Pos;

    while (1)
    {
        // joy_pos_read(&JoyStick, &message, &Pad);
        // b = JoyStick.X;
        // c = JoyStick.Y;
        // d = (int32_t)JoyStick.degrees;
        // e = Pad.X;
        // update_duty_cycle_servo(JoyStick.degrees);
        // game_end(&message);
        // solenoid_activate(&Pad);
        // control_motor(&Mot_Pos, &Pad);
        game(&message, &Mot_Pos, &Pad, &JoyStick);
    }  
}