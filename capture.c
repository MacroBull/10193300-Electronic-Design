#include <msp430.h>
#include <stdint.h>

int16_t cap_val, cap_flag;
int16_t cc0 = 0, cc1, cc_delta;
uint32_t freq = 0;

#define MIN_INT 166
#define MAX_INT 20000

void cap_init(void){
	
	P1DIR |=BIT0;
	
	P1SEL |= BIT2;
	TACCTL1 = CM_1 + CCIS_0 + CAP + CCIE;
}

uint16_t cap_query_block(){
	while (!(CCIFG & TACCTL1));
	TACCTL1 &= ~CCIFG;
	return CCR1;
}

uint16_t cap_query(){
	if (0 == cap_flag){
		cap_val = -1;
	};
	cap_flag = 0;
	return cap_val;
}

uint16_t freq_query(){
	
	return (uint16_t)freq;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
	//cap_val = CCR1;
	//cap_flag = 1;
	
	cc1 = CCR1;
	
	cc_delta =  (cc1 - cc0) & 0x7fff;
	
	if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)){
		cc0 = cc1;
		freq = cc_delta;
		freq = SMCLK_FREQ / freq;
	}
	else {
		;//freq = 0;
	}
	
	TACCTL1 &= ~CCIFG;
	P1OUT ^=BIT0;
}
