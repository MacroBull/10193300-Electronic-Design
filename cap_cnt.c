#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"


#define MAXINT 0x7fff // 最大捕获等待间隔

#define MIN_INT (SMCLK_FREQ / OUT_FREQ_MAX / 2) //最小间隔(带裕量)
#define MAX_INT (SMCLK_FREQ / OUT_FREQ_MIN * 2) //最小间隔(带裕量)


uint16_t cc0, cc1, cc_delta;	//捕获值, 捕获增量(间隔)
uint16_t int_cnt;				//计数周期数
uint32_t cc_cnt, freq, freq_shadow; //freq == 0 : invalid value //捕获计数数, 频率计算结果和询问影子


void cap_init(){
	
	P1DIR &= ~CAP_IN;
	P1SEL |= CAP_IN;   // SEL=1 SEL2=0 -> capture
	CCTL1 = CM_2 + CCIS_0 + CAP + CCIE; // rising edge
}

uint16_t freq_query(){ // wrapper
	return (uint16_t)freq_shadow; // 只返回影子, 即上一次计算有效值
}


void ccr1_isr(){
	cc1 = CCR1;
	cc_delta =  (cc1 - cc0) & 0x7fff; // for safe 保证delta为正
	
	if ((cc_delta>MIN_INT)&&(cc_delta<MAX_INT)){ // valid edge /带通滤波 限制为有效触发
		cc_cnt ++;
		cc0 = cc1;
	}
}

void calc_freq(){
	
	int_cnt ++;
	if ( cc_cnt * int_cnt > 2000){ //保证计算结果可靠, 误差小于1%
		freq = ((1<<8) + SMCLK_FREQ) >> 16;
		freq = (freq * cc_cnt + (int_cnt >> 1)) / int_cnt;
	}
	if ((cc_cnt > 0x800000) || (int_cnt >40)){ // 重新计数, 更新影子
		freq_shadow = freq;
		cc_cnt = 0;
		int_cnt = 0;
		freq = 0;
	}
}
