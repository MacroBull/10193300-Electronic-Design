#include <msp430.h>
#include <stdint.h>

#include "pin.h"


#define MIN_INT (SMCLK_FREQ / 3600) //+20%
#define MAX_INT (SMCLK_FREQ / 160)  //-20%



uint16_t cc0 = 0, cc1, cc_delta;
uint32_t freq = 0, cnt; //freq == 0 : invalid input


void cap_init(void){
	
	P1DIR &= ~PIN_CAP;
	P1REN |= PIN_CAP;
	CCTL2 = CM_1 + CCIS_0 + CAP + CCIE;
}

uint16_t freq_query(){ // encapsulation
	return (uint16_t)freq;
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void captureAndRefresh (void)
{
	
	switch(TAIV){
		case	4: //CCR1
			cc1 = CCR2;
			
			CCTL2 &= ~CCIFG;
			P1OUT &= ~PIN_CAP;
			break;
			
		case	10:
			display(cc1);
			freq = 0;
			TACTL &= ~TAIFG;
			
			P1OUT |= PIN_CAP;
			P1SEL |= PIN_CAP;
			P1SEL2 |= PIN_CAP;
			
			break;
	}
	
}
