#include <msp430.h>
//#include <stdint.h>
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

void display_init(){
	
	P1DIR = BIT0+BIT6+BIT7;
	P2DIR = 0xff;
	P2SEL = P2SEL2 = 0x0;
}

void display(int n){

	P1OUT = BIT0;
	P2OUT = SEG_DISP_CA_7_NUM[(n/100) & 0xf];
	__delay_cycles(DISP_INT);
	P2OUT = SEG_DISP_CA_7_Z;
	P1OUT = BIT6;
	P2OUT = SEG_DISP_CA_7_NUM[(n/10 % 10) & 0xf];
	__delay_cycles(DISP_INT);
	P2OUT = SEG_DISP_CA_7_Z;
	P1OUT = BIT7;
	P2OUT = SEG_DISP_CA_7_NUM[(n % 10) & 0xf];
	__delay_cycles(DISP_INT);
	P2OUT = SEG_DISP_CA_7_Z;
	
}
	
