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

#define MODE_DIRECT 0
#define MODE_NOTE 1
#define MODE_OFF 2

#define JITTER_DELAY 60
#define SWITCH_DELAY 120

char mode = MODE_DIRECT;

uint16_t input_freq, output_freq, last_output_freq, disp_freq;

uint16_t counter;

void main_action(){
	
// 	touch_test();
// 	display(int2disp(key1.counter* 10) );
	
	if (touch_pressed(&key0))
		mode = MODE_DIRECT;
	else if (touch_pressed(&key1))
		mode = MODE_NOTE;
	
	if (click_pressed()){
		click_disable();
		counter = JITTER_DELAY;
		
		mode ^= MODE_OFF;
		if (mode < MODE_OFF) display_enable();
		else display_disable();
	}
	
	if (mode < MODE_OFF) {
		input_freq = freq_query();
		
		if (MODE_DIRECT == mode){
			display(int2disp(input_freq));
			if ((input_freq < 200) || (input_freq > 3000)) output_freq = 0;
			else output_freq = input_freq;
		}
		else if (MODE_NOTE == mode){
				display(note2disp(input_freq));
				output_freq = get_note();
		}
	}
	
	if ((0==last_output_freq)&&(0<output_freq)) {
		click_disable();
		gen_start();
	}
	else if ((0<last_output_freq)&&(0==output_freq)) { 
		gen_stop();
		counter = SWITCH_DELAY;
	}
	last_output_freq = output_freq;
	
	if (output_freq) 
		gen_set(output_freq);
	else if (1 == counter)
		click_enable();
	else if (counter) 
		counter--;
	
// 	WDT_GO;
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
	__delay_cycles(SMCLK_FREQ >> 2); // wait for stability
	
	display_enable();
	
// 	gen_set(4000);
	
	
	_BIS_SR(CPUOFF + GIE);
	
	
	
	
}

