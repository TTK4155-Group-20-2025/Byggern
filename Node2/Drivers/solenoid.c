#include "sam.h"
#include "solenoid.h"

void solenoid_init() {
    PMC->PMC_PCER0 |= 1 << ID_PIOD;
    PIOD->PIO_PER = PIO_PER_P5;
    PIOD->PIO_OER = PIO_OER_P5;

    PIOD->PIO_SODR = PIO_SODR_P5;
}

void solenoid_activate(pad_t* pad) {
    if (pad->button) {
        ADC->ADC_IDR |= ADC_IDR_COMPE;
	    NVIC_DisableIRQ(ID_ADC);
        PIOD->PIO_CODR = PIO_SODR_P5;
    } else {
        PIOD->PIO_SODR = PIO_CODR_P5;
        ADC->ADC_IER |= ADC_IER_COMPE;
        NVIC_EnableIRQ(ADC_IRQn);
    }
}