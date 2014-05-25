#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"

char click_flag, click_enabled;

void click_enable(){
	
	P1DIR &= ~CLICK_IN; //输入
	P1SEL &= ~CLICK_IN; //普通IO
	P1IES |= CLICK_IN;  //下降沿
	P1IE |= CLICK_IN;   //启用中断
	click_enabled = 1;
}

void click_disable(){
	P1IE &= ~CLICK_IN; //禁用中断
	click_enabled = 0;
}

char click_pressed(){
	char tmp = click_flag; //读取并清除标记
	click_flag = 0;
	return click_enabled & tmp;
}

void clickEvent()
{
	if (CLICK_IN & P1IFG){ //记录点击标记
		click_flag = 1;
		P1IFG &= ~CLICK_IN;
	}
}
