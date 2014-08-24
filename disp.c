#include <msp430.h>
#include <stdint.h>
/*
 * 数码管引脚
 * 	CA1	A	F	CA2	CA3	B
 * 	6	7	8	9	10	11
 * PIN
 * 	1	2	3	4	5
 * 	D	D	DOT	C	G
 *  
 * 
 */


#include "driver/seg_disp/8seg.h"	

#include "pin.h"
#include "header.h"


#define DISP_INTV 0.5	//更新间隔

#define MAX_DISP_PERIOD ( (int)((SMCLK_FREQ >>16)*DISP_INTV) )	//换算更新周期


uint8_t disp_digit=0;	//当前显示位
uint16_t disp_cnt;	//更新间隔计数

static uint16_t note_output; //频率吸附至对应音符输出

disp_arr disp_shadow, tmp_arr; //显示缓存

void display_init(){
	
	P2DIR = 0xff;
	P2SEL = P2SEL2 = 0x0;
	
	TACTL |= TAIE;
	
}

void display_enable(){
	
	P1DIR |= DISP_O0 + DISP_O1 +DISP_O2;
}

void display_disable(){
	
	P1DIR &= ~(DISP_O0 + DISP_O1 +DISP_O2);
}

void display(disp_arr n){ // 显示数码管显示
	
	disp_cnt++;
	if (disp_cnt > MAX_DISP_PERIOD){ //计数到周期则更新
		disp_shadow[0] = n[0];
		disp_shadow[1] = n[1];
		disp_shadow[2] = n[2];
		disp_cnt = 0;
	}
	
// 	P2OUT = SEG_DISP_CA_7_Z;
	switch (disp_digit){ // 显示对应位
		case	0:
			P1OUT &= ~DISP_O2;
			P2OUT = disp_shadow[0];
			P1OUT |= DISP_O0;
			disp_digit = 1;
			break;
		case	1:
			P1OUT &= ~DISP_O0;
			P2OUT = disp_shadow[1];
			P1OUT |= DISP_O1;
			disp_digit = 2;
			break;
		case	2:
			P1OUT &= ~DISP_O1;
			P2OUT = disp_shadow[2];
			P1OUT |= DISP_O2;
			disp_digit = 0;
			break;
	}
	
	
}

uint8_t *int2disp(uint16_t n){ //转换整数至显示段
	n = (5 + n) / 10; // 格式1.00kHz
	
	
	tmp_arr[0] = SEG_DISP_CA_7_NUM[n % 10];
	tmp_arr[1] = SEG_DISP_CA_7_NUM[n /10 % 10];
	tmp_arr[2] = SEG_DISP_CA_7_NUM[0xf & (n /100)] ^ DOT;

	return tmp_arr;

}

uint8_t *note2disp(uint16_t freq){ //转换频率为对应音符
	
	const uint16_t noteFreqs[] = {
		4186, 4699, 5274, 5588, 6272, 7040, 7902, 0x7fff
	}; //给定第七八度的音符频率
	
	#define OCT_OFFSET 7
	#define DISP_LOW 0b11110111	//显示频率过低
	#define DISP_HIGH 0b11111110	//显示频率过高
	
	const uint8_t noteDisp[] = {
		SEG_DISP_CA_7_NUM[0xc],
		SEG_DISP_CA_7_NUM[0xd],
		SEG_DISP_CA_7_NUM[0xe],
		SEG_DISP_CA_7_NUM[0xf],
		0b11000010,
		SEG_DISP_CA_7_NUM[0xa],
		SEG_DISP_CA_7_NUM[0xb],
		DISP_HIGH,
	}; //音符CDEFGAB
	
	freq++; //处理无效输入
	uint8_t note = 0, oct = 0;
	while ((freq << oct)< noteFreqs[0]) oct++; //调整至对应octave
	freq = freq << oct;
	while (freq> noteFreqs[note+1]) note++; //线性查表
	
	if (oct > OCT_OFFSET - 4){	//过低
		tmp_arr[0] = DISP_LOW;
		tmp_arr[1] = SEG_DISP_CA_7_Z;
		note_output = 0;
	}
	else if (oct < 1){	//过高
		tmp_arr[0] = DISP_HIGH;
		tmp_arr[1] = SEG_DISP_CA_7_Z;
		note_output = 0;
	}
	else { //正常则显示note & octave
		tmp_arr[0] = noteDisp[note];
		tmp_arr[1] = SEG_DISP_CA_7_NUM[OCT_OFFSET - oct];
		note_output = noteFreqs[note] >> oct;
	}
	tmp_arr[2] = 0b00101011; // 显示 'n' for note

	return tmp_arr;

}

uint16_t get_note(){ //返回对应音符输出频率
	return note_output;
}
