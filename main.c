#include <avr/io.h>
//#include <util/delay.h>

int main() {
    DDRA = (1 << DDA0);
    while (1) {
        PORTA = (1 << PA0);
        for (int i = 0; i < 25000; i++) {
        }

        PORTA = (0 << PA0);
    
        for (int i = 0; i < 25000; i++) {
        }

        //PORTA = (1 << PA0);
    }
    return 0;
}