#include <msp430.h>
#include <stdint.h>

#include "pin.h"

uint16_t period_inc;	// CCR0递增计数值, 即半周期值

void gen_init(){
	
	
	TACTL = TASSEL_2 + MC_2; //连续计数模式
	
}

void gen_start(){
	P1DIR |= SQR_OUT;
	P1SEL |= SQR_OUT;

	CCTL0 = OUTMOD_4 + CCIE; //Togle output (翻转输出)
}

void gen_set(uint16_t freq){
	period_inc = (uint16_t)((SMCLK_FREQ / (uint32_t)freq + 1) >> 1); //计算半周期值
}

void gen_stop(){
	P1DIR &= ~SQR_OUT;
	CCTL0 &= ~(OUTMOD_4 + CCIE);
}

void ccr0_isr(){
	CCR0 += period_inc; //CCR0的ISR, 递增计数
}
