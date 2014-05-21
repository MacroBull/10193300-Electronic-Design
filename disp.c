#include <msp430.h>
#include <stdint.h>
/*
 * 
 * 	CA1	B0	B5	CA2	CA3	B1
 * 	6	7	8	9	10	11
 * PIN
 * 	1	2	3	4	5
 * 	B4	B3	DOT	B2	B6
 *  
 * 
 */


#include "seg_disp/8seg.h"

#include "pin.h"


#define DISP_INTV 0.1

#define MAX_DISP_PERIOD ( (int)((SMCLK_FREQ >>16)*DISP_INTV) )

uint8_t disp_digit=0;
uint16_t disp_cnt, disp_shadow;

void display_init(){
	
	P1DIR |= DISP_O0 + DISP_O1 +DISP_O2;
	P2DIR = 0xff;
	P2SEL = P2SEL2 = 0x0;
	
	TACTL |= TAIE;
	
}

void display(uint16_t n){
	
	disp_cnt++;
	if (disp_cnt > MAX_DISP_PERIOD){ // display 1/10 n
		disp_cnt = 0;
		disp_shadow = (5 + n) / 10;
	}
	
	P2OUT = SEG_DISP_CA_7_Z;
	switch (disp_digit){
		case	0:
			P1OUT &= ~DISP_O2;
			P1OUT |= DISP_O0;
			P2OUT = SEG_DISP_CA_7_NUM[(disp_shadow % 10)& 0xf];
			disp_digit = 1;
			break;
		case	1:
			P1OUT &= ~DISP_O0;
			P1OUT |= DISP_O1;
			P2OUT = SEG_DISP_CA_7_NUM[(disp_shadow /10 % 10)& 0xf];
			disp_digit = 2;
			break;
		case	2:
			P1OUT &= ~DISP_O1;
			P1OUT |= DISP_O2;
			P2OUT = SEG_DISP_CA_7_NUM[(disp_shadow / 100)& 0xf] ^ DOT;
			disp_digit = 0;
			break;
	}
	
	
}

