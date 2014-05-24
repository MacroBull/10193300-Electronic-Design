#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"

char click_flag, click_enabled;

void click_enable(){
	
	P1DIR &= ~CLICK_IN;
	P1SEL &= ~CLICK_IN;
	P1IES |= CLICK_IN;
	P1IE |= CLICK_IN;
	click_enabled = 1;
}

void click_disable(){
	P1IE &= ~CLICK_IN;
	click_enabled = 0;
}

char click_pressed(){
	char tmp = click_flag;
	click_flag = 0;
	return click_enabled & tmp;
}

void clickEvent()
{
	if (CLICK_IN & P1IFG){
		click_flag = 1;
		P1IFG &= ~CLICK_IN;
	}
}
