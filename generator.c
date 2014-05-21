#include <msp430.h>
#include <stdint.h>

#include "pin.h"

uint16_t period_inc;

void gen_init(){
	
	P1DIR |= SQR_OUT;
	P1SEL |= SQR_OUT;
	
	TACTL = TASSEL_2 + MC_2;
	
}

void gen_start(){
	CCTL0 = OUTMOD_4 + CCIE;
}

void gen_set(uint16_t freq){
	period_inc = (uint16_t)((SMCLK_FREQ / (uint32_t)freq + 1) >> 1);
}

void gen_stop(){
	CCTL0 &= ~(OUTMOD_4 + CCIE);
}

void ccr0_isr(){
	CCR0 += period_inc;
}
