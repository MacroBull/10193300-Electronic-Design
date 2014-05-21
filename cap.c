#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"


#define MAXINT 0x7fff

#define MIN_INT (SMCLK_FREQ / 3000 / 4 ) //+20%
// #define MIN_INT (SMCLK_FREQ / 4 / 7200 ) //+20%
// #define MAX_INT (SMCLK_FREQ * 3 / 160 )  //-20%
#define MAX_INT MAXINT

#define MAX_CCNT 16


uint16_t cc0, cc1, cc_delta, cc_cnt;
uint32_t cc_sum;
uint32_t freq = 0, cnt; //freq == 0 : invalid value


void cap_init(){
	
	P1DIR &= ~CAP_IN;
	P1SEL |= CAP_IN;
	CCTL1 = CM_3 + CCIS_0 + CAP + CCIE; // rising edge
}

uint16_t freq_query(){ // wrapper
	return (uint16_t)freq;
}

void ccr1_isr(){
	cc1 = CCR1;
	cc_delta =  (cc1 - cc0) & 0x7fff; // for safe
	
	if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)) {// valid edge
		cc_sum += cc_delta;
		cc_cnt ++;
	}
}

void calc_freq(){
	
	freq = ( SMCLK_FREQ  * cc_cnt + (cc_sum >> 1) ) / cc_sum;
	
	cc_sum = 0;
// 	appendData(MAXINT);
	
}
