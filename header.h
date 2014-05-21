#include <stdint.h>


#ifndef __HEADER_H
#define __HEADER_H

//Display
extern void display_init();
extern void display(uint16_t n);
extern void update_display();

//Input capture
extern void cap_init();
extern uint16_t freq_query();
extern void calc_freq();

//Square Output
extern void gen_init();
extern void gen_start();
extern void gen_set(uint16_t freq);
extern void gen_stop();

//Touch key
typedef struct {
	uint8_t pin;
	uint16_t counter;
	uint16_t key_cd;
}touch_key;

typedef touch_key *touch_key_handle;


touch_key key0, key1;

extern void touch_init();
extern char touch_pressed(touch_key_handle key);
extern void toggle_touch_output();

//ISR proxy
extern void port1_isr();
extern void ccr0_isr();
extern void ccr1_isr();
extern void ccr2_isr();
extern void ta_isr();

#endif