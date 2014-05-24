/*
 * 
 * 
 * 
 * 
 */

#include <msp430.h>
#include "calib_val.h"
#include "wdt.h"
#include "header.h"

#define MODE_DIRECT	0
#define MODE_NOTE	1

char mode;

uint16_t input_freq, output_freq, last_output_freq, disp_freq;
void main_action(){
	
// 	touch_test();
	if (touch_pressed(&key0))
		mode = MODE_DIRECT;
	else if (touch_pressed(&key1))
		mode = MODE_NOTE;
	
// 	display(int2disp(mode * 10));
	
	input_freq = freq_query();
	switch (mode){
		case MODE_DIRECT:
			display(int2disp(input_freq));
			output_freq = input_freq;
			if ((output_freq < 200) || (output_freq > 3000)) output_freq = 0;
			break;
		case MODE_NOTE:
			display(note2disp(input_freq));
			output_freq = get_note();
			break;
	}
	
	if ((0==last_output_freq)&&(0<output_freq)) gen_start();
	if ((0<last_output_freq)&&(0==output_freq)) gen_stop();
	if (output_freq) gen_set(output_freq);
	last_output_freq = output_freq;
}

void main(){
	
	BC16MSET;
	WDT_DISABLE;
	
 	P1DIR |= BIT1;
	
 	gen_init();
 	display_init();
	touch_init();
 	cap_init();
	
	
	last_output_freq = 1;
	__delay_cycles(SMCLK_FREQ >> 1);
// 	gen_set(4000);
	
	
	_BIS_SR(CPUOFF + GIE);
	
	
	
	
}

