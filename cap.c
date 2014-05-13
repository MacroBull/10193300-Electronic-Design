#include <msp430.h>
#include <stdint.h>

#include "pin.h"


#define MIN_INT (SMCLK_FREQ / 4 / 7200 ) //+20%
#define MAX_INT (SMCLK_FREQ * 3 / 160 )  //-20%

#define MAX_CCNT 16

#define MAXINT 0xffff

uint16_t cc0, cc1, cc_delta, ccnt;
uint16_t cdval[MAX_CCNT];
uint32_t freq = 0, cnt; //freq == 0 : invalid input


void cap_init(void){
	
	P1DIR &= ~PIN_CAP;
	P1SEL |= PIN_CAP;
	CCTL1 = CM_3 + CCIS_0 + CAP + CCIE;
}

uint16_t freq_query(){ // encapsulation
	return (uint16_t)freq;
}

void appendData(uint16_t data){
	cdval[ccnt] = data;
	cc0 = cc1;
	ccnt++;
	ccnt &=0xf;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void captureAndRefresh (void)
{
	
	switch(TAIV){
		case	2: //CCR1
			cc1 = CCR1;
			cc_delta =  (cc1 - cc0) & 0x7fff; // for safe
			
			if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT))
				appendData(cc_delta);
			
			CCTL1 &= ~CCIFG;
			break;
			
		case	10:
			freq = 0;
			
			int i;
			for (i = 0;i<MAX_CCNT;i++){
				freq += cdval[i];
			}
			
			freq = ( (SMCLK_FREQ * 8 )+freq/2) / freq;
			display((uint16_t)freq);
			appendData(MAXINT);
			
			TACTL &= ~TAIFG;
			break;
	}
	
}
