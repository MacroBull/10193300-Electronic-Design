#include <msp430.h>
#include <stdint.h>

#include "pwm/pwm.h"

#include "def/perip/g2452.h"
#include "def/pin/exp430g2.h"


#define	SQR_OUT	5
#define	SQR_OUT_BIT	BIT5

void gen_init(){
	PWM0->OUT0 = SQR_OUT;
	PWM_enable(PWM0, SMCLK, UP, BIT0, 0);
	P1SEL &= ~SQR_OUT_BIT;
}

void gen_start(uint16_t freq){
	P1SEL |= SQR_OUT_BIT;
	PWM_setter(PWM0, 0, (uint16_t)((SMCLK_FREQ / (uint32_t)freq + 1) >> 1), TOGGLE);
}

void gen_stop(){
	P1SEL &= ~SQR_OUT_BIT;
}
	
