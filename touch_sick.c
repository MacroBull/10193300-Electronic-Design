
#include <msp430.h>
#include <stdint.h>

#include "pin.h"
#include "header.h"

#define COLDDOWN 20




void isr_handle(touch_key_handle key){
	key->counter = TAR;
	
	P1IE &= ~(key->pin);
	P1IFG &= ~(key->pin);
	
}

void update(touch_key_handle key){
	if (0 == key->counter) 
		key->key_cd = COLDDOWN;
	else 
		if (key-> key_cd > 0) key->key_cd --;
	key->counter = 0;
}

char touch_pressed(touch_key_handle key){
	return (key-> key_cd > 0);
}


void touch_init(){
	
	key0.pin = TOUCH_PIN0;
	key1.pin = TOUCH_PIN1;
	
	P1DIR &= ~(TOUCH_PIN0 + TOUCH_PIN1);
	P1DIR |= TOUCH_SOUT;
	P1IE = 0;
// 	P1IES &= ~(TOUCH_PIN0 + TOUCH_PIN1);
	P1IES |= (TOUCH_PIN0 + TOUCH_PIN1);
	
	TACTL = TASSEL_2 + MC_2 + TAIE;
	
}

void toggle_touch_output(){
	
	P1OUT ^= TOUCH_SOUT;
	
	if (P1OUT & TOUCH_SOUT){
		update(&key0);
		update(&key1);
		P1IE |= (TOUCH_PIN0 + TOUCH_PIN1);
	}
	
}

void port1_isr(){
//  	P1OUT ^= BIT1;
	if (key0.pin & P1IFG) isr_handle(&key0);
	if (key1.pin & P1IFG) isr_handle(&key1);
	
}


