/*
 * 
 * 
 * 
 * 
 */

#include <msp430.h>
#include "wdt.h"

void main(){
	
	WDT_DISABLE;
	P1DIR |= ~0;
	while (1){
		P1OUT ^= ~0;
		__delay_cycles(1000000);
	}
}
