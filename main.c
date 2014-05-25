/*
 * 
 * https://github.com/MacroBull/10193300-Electronic-Design
 * 
 */

#include <msp430.h>
#include "calib_val.h"
#include "wdt.h"
#include "header.h"

#define MODE_DIRECT 0	//直接显示频率
#define MODE_NOTE 1		//换算音符频率
#define MODE_OFF 2		//关机

#define JITTER_DELAY 60		//敲击防抖
#define SWITCH_DELAY 120	//敲击启用延时

char mode = MODE_NOTE;	//默认显示音符

uint16_t input_freq, output_freq, last_output_freq, disp_freq;	//各种频率值

uint16_t counter;	//总循环计数

void main_action(){
	
// 	touch_test();
// 	display(int2disp(key1.counter* 10) );
	
	if (touch_pressed(&key0)) //按键切换模式
		mode = MODE_DIRECT;
	else if (touch_pressed(&key1))
		mode = MODE_NOTE;
	
	if (click_pressed()){	//点击切换开关
		click_disable();
		counter = JITTER_DELAY;
		
		mode ^= MODE_OFF;
		if (mode < MODE_OFF) display_enable();
		else display_disable();
	}
	
	if (mode < MODE_OFF) {	//开启
		input_freq = freq_query();
		
		if (MODE_DIRECT == mode){ //显示频率
			display(int2disp(input_freq));
			if ((input_freq < 200) || (input_freq > 3000)) output_freq = 0; //无效频率不输出
			else output_freq = input_freq;
		}
		else if (MODE_NOTE == mode){ //显示音符
				display(note2disp(input_freq));
				output_freq = get_note();
		}
	}
	
	if ((0==last_output_freq)&&(0<output_freq)) { //启动输出
		click_disable();
		gen_start();
	}
	else if ((0<last_output_freq)&&(0==output_freq)) { //关闭输出
		gen_stop();
		counter = SWITCH_DELAY;
	}
	last_output_freq = output_freq;
	
	if (output_freq) 
		gen_set(output_freq); //设置输出频率
	else if (1 == counter)
		click_enable();	//延迟到位, 启用敲击
	else if (counter) 
		counter--;
	
// 	WDT_GO;
}

void initSysctl(){
	BC16MSET;
	WDT_DISABLE;
}

void initPerip(){
	//  P1DIR |= BIT1;
	
	gen_init();
	display_init();
	touch_init();
	cap_init();
	
}

void _main(){
	initSysctl();
	initPerip();
	
	last_output_freq = 1;
	__delay_cycles(SMCLK_FREQ >> 2); // wait for stability 等待捕获, 触摸和敲击稳定
	
	display_enable();
	
	_BIS_SR(CPUOFF + GIE);
}

