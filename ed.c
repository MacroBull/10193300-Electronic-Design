#include <msp430.h>
#include <stdint.h>

#include "wdt.h"
#include "calib_val.h"

#include "misc.h"

void main(){
	WDT_DISABLE;
	BC1MSET;
	
	display_init();
	gen_init();
	gen_start(1000);
	cap_init();
	
	
	_EINT();
	
	
	while (1){
		
		display(freq_query()/100);
	}
	
	_BIS_SR(CPUOFF);
}
