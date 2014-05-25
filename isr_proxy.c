#include <msp430.h>

#include "header.h"


// // Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_TouchKey(void)
{
	touchEvent();	//检测触摸
	clickEvent();	//检测敲击
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void CCR0_update (void)
{
	ccr0_isr();
}

// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA0_Multifunction(void)
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


inline void ta_isr(){
	calc_freq();	//计算频率
	main_action();	//主循环
	toggle_touch_output();	//翻转触摸信号
}