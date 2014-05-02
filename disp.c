#include <msp430.h>
#include <stdint.h>
/*
 * 
 * 	CA1	B0	B5	CA2	CA3	B1
 * 	6	7	8	9	10	11
 * PIN
 * 	1	2	3	4	5
 * 	B3	B4	DOT	B2	B6
 *  
 * 
 */


#include "seg_disp/8seg.h"

#define DISP_INT 1000*200
#define MAX_DISP_PERIOD ( (int)((SMCLK_FREQ >>16)/3*0.5) )

uint8_t digit=0;
uint16_t disp_cnt, freq_in_disp;

void display_init(){
	
	P1DIR |= BIT5+BIT6+BIT7;
	P2DIR = 0xff;
	P2SEL = P2SEL2 = 0x0;
	
	TACTL |= TAIE;
	
	
}

void display(uint16_t n){
	
	disp_cnt++;
	if (disp_cnt > MAX_DISP_PERIOD){
		disp_cnt = 0;
		freq_in_disp = n;
	}
	
	P2OUT = SEG_DISP_CA_7_Z;
	switch (digit){
		case	0:
			P1OUT = BIT7;
			P2OUT = SEG_DISP_CA_7_NUM[(freq_in_disp % 10)& 0xf];
			digit = 1;
			break;
		case	1:
			P1OUT = BIT6;
			P2OUT = SEG_DISP_CA_7_NUM[(freq_in_disp /10 % 10)& 0xf];
			digit = 2;
			break;
		case	2:
			P1OUT = BIT5;
			P2OUT = SEG_DISP_CA_7_NUM[(freq_in_disp / 100)& 0xf] ^ DOT;
			digit = 0;
			break;
	}
	
	
}
	
