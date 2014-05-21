#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"


#define MIN_INT (SMCLK_FREQ / 7200 ) //+20%
// #define MIN_INT (SMCLK_FREQ / 4 / 7200 ) //+20%
#define MAX_INT (SMCLK_FREQ * 3 / 160 )  //-20%

#define MAX_CCNT 16

#define MAXINT 0xffff

uint16_t cc0, cc1, cc_delta, ccnt;
uint16_t cdval[MAX_CCNT];
uint32_t freq = 0, cnt; //freq == 0 : invalid value


void cap_init(){
	
	P1DIR &= ~CAP_IN;
	P1SEL |= CAP_IN;
	CCTL1 = CM_3 + CCIS_0 + CAP + CCIE; // rising edge
}

uint16_t freq_query(){ // wrapper
	return (uint16_t)freq;
}

void appendData(uint16_t data){
	cdval[ccnt ++] = data;
	cc0 = cc1;
	ccnt &=0xf;
}

void ccr1_isr(){
	cc1 = CCR1;
	cc_delta =  (cc1 - cc0) & 0x7fff; // for safe
	
	if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)) // valid edge
		appendData(cc_delta);
}

void calc_freq(){
	
	freq = 0;
	
	int i;
	for (i = 0;i<MAX_CCNT;i++){
		freq += cdval[i];
	}
	
	freq = ( (SMCLK_FREQ << 4 ) + (freq >> 1) ) / freq;
	
	cc1 = 0;
	appendData(MAXINT);
	
}
