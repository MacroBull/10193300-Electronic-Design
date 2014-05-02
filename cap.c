#include <msp430.h>
#include <stdint.h>

#include "pin.h"


#define MIN_INT (SMCLK_FREQ / 3600) //+20%
#define MAX_INT (SMCLK_FREQ / 160)  //-20%



uint16_t cc0 = 0, cc1, cc_delta;
uint32_t freq = 0, cnt; //freq == 0 : invalid input


void cap_init(void){
	
	P1SEL |= PIN_CAP;
	CCTL1 = CM_1 + CCIS_0 + CAP + CCIE;
}

uint16_t freq_query(){ // encapsulation
	return (uint16_t)freq;
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void captureAndRefresh (void)
{
	
	switch(TAIV){
		case	2: //CCR1
			cc1 = CCR1;
			cc_delta =  (cc1 - cc0) & 0x7fff; // for safe
			
			if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)){
				cc0 = cc1;
				freq = cc_delta;
				freq = (SMCLK_FREQ+freq/2) / freq;
			}
			else {
				freq = 0;
			}
			
			CCTL1 &= ~CCIFG;
			break;
			
		case	10:
			display((uint16_t)freq);
			freq = 0;
			TACTL &= ~TAIFG;
			break;
	}
	
}
