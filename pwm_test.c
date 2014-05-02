#include <msp430.h>


#include "wdt.h"
#include "calib_val.h"
//#include "spwm_table_65536_1024.h"

#include "pwm/pwm.h"

#include "pwm/spwm/spwm_table_65536_1024.h"

#include "def/perip/g2452.h"
#include "def/pin/exp430g2.h"



void main(){
	WDT_DISABLE;
	BC1MSET;
	
	PWM_enable(PWM0, SMCLK, UP, BIT0+BIT1, 0x8000);
	
	
	PWM_setter(PWM0, 0, 0x8000, TOGGLE);
	//CCTL0 = OUTMOD_4;
	//PWM_setter(PWM0, 1, 0x1ffd, POUT);
	//PWM_setter(PWM0, 2, 0x1ffd, POUT);
	
	
	_BIS_SR(CPUOFF);
}
