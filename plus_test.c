#include <msp430.h>
#include <stdint.h>

#include "wdt.h"
#include "calib_val.h"

void main(){
	WDT_DISABLE;
	BC1MSET;
	P1DIR = 0xff;
	P1SEL = 0xff;
	P1OUT = BIT0;
	
	CCTL0 = OUTMOD_4 + CCIE; 
	TACTL = TASSEL_2 +  MC_2;
	
	_EINT();
	
	_BIS_SR(CPUOFF);
	
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	CCR0 += 1000;
	P1OUT ^= BIT0;

}
