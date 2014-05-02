#include <msp430.h>
#include <stdint.h>

#include "wdt.h"
#include "calib_val.h"

#include "misc.h"

void main(){
	WDT_DISABLE;
	BC16MSET;
	
	gen_init();
	cap_init();
	display_init();
	gen_start();
	
	
	_EINT();
	
	uint16_t i=200;
	while (1){
		i++;
		if (i>800) i=200;
		gen_set(i);
		__delay_cycles(300000);
	}
	
	
	_BIS_SR(CPUOFF);
}
