#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "StarterCode/uart.h"
#include "CAN_node_2/can_controller.h"
#include "Drivers/joystick.h"
#include "Drivers/Peripherals/pwm.h"
#include "Drivers/photodiode.h"

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
    pwm_init();
    photodiode_init();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // PMC->PMC_PCER0 = PMC_PCER0_PID12; // Just for testing

    // PIOB->PIO_PER = PIO_PER_P13;
    // PIOB->PIO_OER = PIO_OER_P13;

    // PIOB->PIO_SODR = PIO_SODR_P13;

    // PIOB->PIO_CODR = PIO_SODR_P13;

    int16_t b = (int16_t)0;
    int16_t c = (int16_t)0;
    int32_t d = (int32_t)0;
    uint32_t e = 0;
    CAN_MESSAGE message;
    joystick_pos JoyStick;

    while (1)
    {
        joy_pos_read(&JoyStick, &message);
        b = JoyStick.X;
        c = JoyStick.Y;
        d = (int32_t)JoyStick.degrees;
        printf("X: %6ld Y: %6ld Degrees: %6ld\n", b, c, d);
        update_duty_cycle(JoyStick.degrees);
        // e = photodiode_test();
        // printf("Digital value of photodiode: %4lu\n", e);
        game_end(&message);
    }
    
}