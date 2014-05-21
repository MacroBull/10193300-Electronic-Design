#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"


#define MIN_INT (SMCLK_FREQ / 7200 ) //+20%
// #define MIN_INT (SMCLK_FREQ / 4 / 7200 ) //+20%
#define MAX_INT (SMCLK_FREQ * 3 / 160 )  //-20%

#define MAX_CCNT 16

#define MAXINT 0xffff

uint16_t cc0, cc1, cc_delta, cc_cnt;
uint32_t freq; //freq == 0 : invalid value


void cap_init(){
	
	P1DIR &= ~CAP_IN;
	P1SEL |= CAP_IN;
	CCTL1 = CM_1 + CCIS_0 + CAP + CCIE; // rising edge
}

uint16_t freq_query(){ // wrapper
	return (uint16_t)freq;
}


void ccr1_isr(){
	cc1 = CCR1;
	cc_delta =  (cc1 - cc0) & 0x7fff; // for safe
	
	if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)){ // valid edge
		cc_cnt ++;
		cc0 = cc1;
	}
}

void calc_freq(){
	freq = cc_cnt;
	freq *=SMCLK_FREQ;
	freq >>= 16;
	cc_cnt = 0;
}
