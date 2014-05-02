#include <msp430.h>
#include <stdint.h>

int16_t cap_val, cap_flag;
int16_t cc0 = 0, cc1, cc_delta;
uint32_t freq = 0, cnt;


#define MIN_INT (SMCLK_FREQ / 3600)
#define MAX_INT (SMCLK_FREQ / 160)

void cap_init(void){
	
	P1DIR |=BIT0;
	P1SEL |= BIT2;
	CCTL1 = CM_1 + CCIS_0 + CAP + CCIE;
}

uint16_t freq_query(){
	return (uint16_t)freq;
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
	
	switch(TAIV){
		case	2:
			cc1 = CCR1;
			
			cc_delta =  (cc1 - cc0) & 0x7fff;
			
			if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)){
				cc0 = cc1;
				freq = cc_delta;
				freq = (SMCLK_FREQ+freq/2) / freq;
			}
			else {
				freq = 0;
			}
			P1OUT ^=1;
			CCTL1 &= ~CCIFG;
			break;
		case	10:
			display((uint16_t)(freq+5)/10);
			freq = 0;
			TACTL &= ~TAIFG;
			break;
	}
	
	

	
}
