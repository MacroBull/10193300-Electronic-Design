#include <msp430.h>

#include "header.h"


// // Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_TouchKey(void)
{
	port1_isr();
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void CCR0_update (void)
{
	ccr0_isr();
}

// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A_Multifunction(void)
{
	switch( TA0IV )
	{
		case  2: 
// 			TACTL &= ~TAIFG;
			/*P1IE = 0;
			TACCTL1 &= ~CCIFG;
			_EINT();*/ // makes CCR0 & display in real time
			ccr1_isr();
			break;
		case  4: 
// 			ccr2_int();
			break;
		case 10: 
// 			TACTL &= ~TAIFG;
			/*P1IE = 0;
			TACCTL1 &= ~CCIFG;
			_EINT();*/ // makes CCR0 & display in real time
			ta_isr();
			break;
	}
	
}


void ta_isr(){
	calc_freq();
	main_action();
 	toggle_touch_output();
}