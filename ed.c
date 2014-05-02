#include <msp430.h>
#include <stdint.h>

#include "wdt.h"
#include "calib_val.h"


void main(){
	
	WDT_DISABLE;
	BC16MSET;
	
	gen_init();
	cap_init();
	display_init();
	
	
	_EINT();
	
	
	gen_start();
	
	uint16_t i=200;
	while (1){
		i++;
		if (i>3000) i=200;
		gen_set(i);
		__delay_cycles(300000);
	}
	
	
	_BIS_SR(CPUOFF);
}
