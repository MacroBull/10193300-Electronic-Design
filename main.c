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
	
	if (touch_pressed(&key0))
	display(9900);
	else if (touch_pressed(&key1))
	display(9911);
	else 
	display(freq_query());
}

void main(){
	
	BC16MSET;
	WDT_DISABLE;
	
	gen_init();
	display_init();
	touch_init();
	cap_init();
	
	gen_start();
	gen_set(300);
	_BIS_SR(CPUOFF + GIE);
	
	
	
	
}

