#include "sram.h"

void sram_init() {
    MCUCR = (1 << SRE);
    SFIOR = (1 << XMM2) | (0 << XMM1) | (0 << XMM0);
}