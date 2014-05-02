#include <msp430.h>
#include <stdint.h>

uint16_t period_delta;

void gen_init(){
	
	P1DIR |=BIT1;
	P1SEL |= BIT1;
	
	TACTL = TASSEL_2 + MC_2;
	CCTL0 = OUTMOD_4;

}

void gen_start(){
	CCTL0 |= CCIE;
}

void gen_set(uint16_t freq){
	period_delta = (uint16_t)((SMCLK_FREQ / (uint32_t)freq + 1) >> 1);
}

void gen_stop(){
	CCTL0 &= ~CCIE;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	CCR0 += period_delta;
}
