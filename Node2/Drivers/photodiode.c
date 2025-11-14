#include "photodiode.h"
#include "sam.h"

volatile uint32_t game_ended_flag = 0;

void photodiode_init() {
    // Enable PMC Controller for ADC
    PMC->PMC_PCER1 |= 1 << (ID_ADC - 32);
    PMC->PMC_PCER0 |= 1 << (ID_TC1);
    
    // Enabling and configuring ADC
    ADC->ADC_MR |= (1 << ADC_MR_PRESCAL_Pos) | (1 << ADC_MR_TRACKTIM_Pos) | (1 << ADC_MR_TRANSFER_Pos); // MCK / 4
    ADC->ADC_CHER |= (ADC_CHER_CH0);
    ADC->ADC_MR |= ADC_MR_FREERUN_ON; // ADC starts conversion by itself

    ADC->ADC_EMR |= ADC_EMR_CMPMODE_LOW | (0 << ADC_EMR_CMPSEL_Pos) | (0 << ADC_EMR_CMPFILTER_Pos) | (ADC_EMR_TAG);
    ADC->ADC_CWR |= (ADC_CWR_LOWTHRES(1000));

    ADC->ADC_IER |= ADC_IER_COMPE;
    NVIC_EnableIRQ(ADC_IRQn);
    ADC->ADC_CR |= (ADC_CR_START);

    // Reset clock for game_ended_flag counter
    TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_ACPC_CLEAR | TC_CMR_ASWTRG_SET | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3;
    TC0->TC_CHANNEL[1].TC_RC = 26250*2;
    TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC1_IRQn);
    TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
uint8_t game_end(CAN_MESSAGE* message) {
    if (game_ended_flag > 1000) {
        message->id = GAME_END;
        message->data_length = 1;
        message->data[0] = 0xFF;
        can_send(message, 0);
        printf("Game ended %lu\n", game_ended_flag);
        game_ended_flag = 0;
        return 0;
    } return 1;
}

uint32_t photodiode_test() {
    return (ADC->ADC_LCDR & ADC_LCDR_LDATA_Msk);
}

void ADC_Handler(void) {
    uint32_t status = ADC->ADC_ISR;
    if (status & ADC_ISR_COMPE) {
        game_ended_flag += 1;
    }
}

void TC1_Handler(void) {
    uint32_t status = TC0->TC_CHANNEL[1].TC_SR;
    game_ended_flag = 0;
}