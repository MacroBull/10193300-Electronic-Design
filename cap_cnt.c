#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"


#define MAXINT 0x7fff

#define MIN_INT (SMCLK_FREQ / OUT_FREQ_MAX / 2) 
#define MAX_INT (SMCLK_FREQ / OUT_FREQ_MIN * 2)


uint16_t cc0, cc1, cc_delta;
uint16_t int_cnt;
uint32_t cc_cnt, freq, freq_shadow; //freq == 0 : invalid value


void cap_init(){
	
	P1DIR &= ~CAP_IN;
	P1SEL |= CAP_IN;
	CCTL1 = CM_2 + CCIS_0 + CAP + CCIE; // rising edge
}

uint16_t freq_query(){ // wrapper
	return (uint16_t)freq_shadow;
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
	
	int_cnt ++;
	if ( cc_cnt * int_cnt > 2000){
		freq = ((1<<8) + SMCLK_FREQ) >> 16;
		freq = (freq * cc_cnt + (int_cnt >> 1)) / int_cnt;
	}
	if ((cc_cnt > 0x800000) || (int_cnt >40)){
		freq_shadow = freq;
		cc_cnt = 0;
		int_cnt = 0;
		freq = 0;
	}
}
