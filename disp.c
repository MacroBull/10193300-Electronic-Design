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

#include "header.h"


#define DISP_INTV 0.3

#define MAX_DISP_PERIOD ( (int)((SMCLK_FREQ >>16)*DISP_INTV) )


uint8_t disp_digit=0;
uint16_t disp_cnt;

static uint16_t note_output;


disp_arr disp_shadow, tmp_arr;

void display_init(){
	
	P2DIR = 0xff;
	P2SEL = P2SEL2 = 0x0;
	
	TACTL |= TAIE;
	
}

void display_enable(){
	
	P1DIR |= DISP_O0 + DISP_O1 +DISP_O2;
}

void display_disable(){
	
	P1DIR &= ~(DISP_O0 + DISP_O1 +DISP_O2);
}

void display(disp_arr n){
	
	disp_cnt++;
	if (disp_cnt > MAX_DISP_PERIOD){ // display 1/10 n
		disp_shadow[0] = n[0];
		disp_shadow[1] = n[1];
		disp_shadow[2] = n[2];
		disp_cnt = 0;
	}
	
// 	P2OUT = SEG_DISP_CA_7_Z;
	switch (disp_digit){
		case	0:
			P1OUT &= ~DISP_O2;
			P2OUT = disp_shadow[0];
			P1OUT |= DISP_O0;
			disp_digit = 1;
			break;
		case	1:
			P1OUT &= ~DISP_O0;
			P2OUT = disp_shadow[1];
			P1OUT |= DISP_O1;
			disp_digit = 2;
			break;
		case	2:
			P1OUT &= ~DISP_O1;
			P2OUT = disp_shadow[2];
			P1OUT |= DISP_O2;
			disp_digit = 0;
			break;
	}
	
	
}

uint8_t *int2disp(uint16_t n){
	n = (5 + n) / 10;
	
	
	tmp_arr[0] = SEG_DISP_CA_7_NUM[n % 10];
	tmp_arr[1] = SEG_DISP_CA_7_NUM[n /10 % 10];
	tmp_arr[2] = SEG_DISP_CA_7_NUM[0xf & (n /100)] ^ DOT;

	return tmp_arr;

}

uint8_t *note2disp(uint16_t freq){
	
	
	const uint16_t noteFreqs[] = {
		4186, 4699, 5274, 5588, 6272, 7040, 7902, 0x7fff
	};
	
	#define OCT_OFFSET 7
	#define DISP_LOW 0b11110111
	#define DISP_HIGH 0b11111110
	
	const uint8_t noteDisp[] = {
		SEG_DISP_CA_7_NUM[0xc],
		SEG_DISP_CA_7_NUM[0xd],
		SEG_DISP_CA_7_NUM[0xe],
		SEG_DISP_CA_7_NUM[0xf],
		0b11000010,
		SEG_DISP_CA_7_NUM[0xa],
		SEG_DISP_CA_7_NUM[0xb],
		DISP_HIGH,
	};
	
	freq++;
	uint8_t note = 0, oct = 0;
	while ((freq << oct)< noteFreqs[0]) oct++;
	freq = freq << oct;
	while (freq> noteFreqs[note+1]) note++;
	
	if (oct > OCT_OFFSET - 4){
		tmp_arr[0] = DISP_LOW;
		tmp_arr[1] = SEG_DISP_CA_7_Z;
		note_output = 0;
	}
	else if (oct < 1){
		tmp_arr[0] = DISP_HIGH;
		tmp_arr[1] = SEG_DISP_CA_7_Z;
		note_output = 0;
	}
	else {
		tmp_arr[0] = noteDisp[note];
		tmp_arr[1] = SEG_DISP_CA_7_NUM[OCT_OFFSET - oct];
		note_output = noteFreqs[note] >> oct;
	}
	tmp_arr[2] = 0b00101011; // 'n' for note

	return tmp_arr;

}

uint16_t get_note(){
	return note_output;
}
