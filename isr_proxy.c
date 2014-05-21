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
			ccr1_isr();
			break;
		case  4: 
// 			ccr2_int();
			break;
		case 10: 
			ta_isr();
			break;
	}
	
}


void ta_isr(){
	calc_freq();
	update_display();
	toggle_touch_output();
}