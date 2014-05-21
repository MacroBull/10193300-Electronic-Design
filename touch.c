
#include <msp430.h>
#include <stdint.h>



#define S_OUT BIT0

#define UP_LEVEL 200
#define DOWN_LEVEL 20
#define COLDDOWN 10


typedef struct {
	uint8_t pin;
	uint16_t baseline, counter;
	uint16_t key_cd;
}touch_key;

typedef touch_key *touch_key_handle;


touch_key key0, key1;


void isr_handle(touch_key_handle key){
	key->counter = TAR;
	
	P1IE &= ~(key->pin);
	P1IFG &= ~(key->pin);
	
}

void update(touch_key_handle key){
	
	if (key->counter > key->baseline + UP_LEVEL) {
		key->key_cd = COLDDOWN;
	}
	else {
// 		key->baseline -=DOWN_LEVEL;
		if (key->key_cd) key->key_cd --;
	}
	
	key->baseline = (key->baseline + key->counter) >> 1;
}

char key_pressed(touch_key_handle key){
	return key->key_cd > 0;
}

#define LED0 BIT1


void cap_init(){
	
	key0.pin = BIT3;
	key1.pin = BIT4;
	
	P1DIR &= ~(key0.pin + key1.pin);
	P1DIR |= S_OUT + LED0;
	P1IE = 0;
	P1IES &= ~(key0.pin + key1.pin);
	
	TACTL = TASSEL_2 + MC_2 + TAIE;
	
}

void ta_isr(){

	update(&key0);
	update(&key1);
	
	int ans = 0;
	if (key_pressed(&key0)){
		ans +=1000;
	}
	if (key_pressed(&key1)){
		ans +=200;
	}
	display(key1.counter / 10);
	
	P1OUT ^= S_OUT;
	P1IE |= (key0.pin + key1.pin);
	
}

void port1_isr(){
	if (key0.pin & P1IFG) isr_handle(&key0);
	if (key1.pin & P1IFG) isr_handle(&key1);
	
}


