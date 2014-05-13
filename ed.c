#include <msp430.h>
#include <stdint.h>

#include "wdt.h"
#include "calib_val.h"

#include "header.h"


void main(){
	
	WDT_DISABLE;
	BC16MSET;
	
	gen_init();
	
	
	cap_init();
	display_init();
	
	
	_EINT();
	
	
// 	gen_start();
	
// 	uint16_t i=200;
// 	while (1){
// 		i++;
// 		if (i>3000) i=200;
// 		gen_set(i);
// 		__delay_cycles(200000);
// 	}
	
	char output = 0;
	P1DIR |= BIT0;
	while (1){
		uint16_t f = freq_query();
		if ((f>=200) && (f<=3000)){
			if (0 == output){
				gen_start();
				output = 1;
				P1OUT |= BIT0;
			}
			gen_set(f);
		}
		else{
			if (1 == output){
				gen_stop();
				output = 0;
				P1OUT &= ~BIT0;
			}
		}
		__delay_cycles(200000);
		
	}
	
	
	_BIS_SR(CPUOFF);
}
