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


void update_display(){
	
	if (touch_pressed(&key1))
	display(1111);
	else 
	display(freq_query());
}

void main(){
	
	BC16MSET;
	WDT_DISABLE;
	
	P1DIR |= BIT1;
	//gen_init();
	display_init();
	touch_init();
	cap_init();
	
	gen_start();
	gen_set(300);
	_BIS_SR(CPUOFF + GIE);
	
	
	
	
}

