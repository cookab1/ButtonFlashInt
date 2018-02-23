/*
 * buttonFlash.c
 *
 * Created: 2/21/2018 9:45:16 PM
 *  Author: Andy
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "buttonFlashInt.h"

int stateTable[2][3] = {{1,0,1},
						{2,2,0}};

//lights are off
void off() {
	if((PORTF & 0x0f) != 0)
	PORTF &= 0xf0;
	
	while(1) {
		delay(1000);
		if(checkButton()) {
			return;
		}
	}
}

//rotate the lights being on from 0 - 3 each in turn
void rotate() {
	while(1) {
		PORTF |= 0x1;
		delay(1000);
		PORTF &= 0xf0;
		if(checkButton())
			return;
		
		PORTF |= 0x2;
		delay(1000);
		PORTF &= 0xf0;
		if(checkButton())
			return;
		
		PORTF |= 0x4;
		delay(1000);
		PORTF &= 0xf0;
		if(checkButton())
			return;
		
		PORTF |= 0x8;
		delay(1000);
		PORTF &= 0xf0;
		if(checkButton())
			return;
	}
}

// flash the lights for 200 mSec and off for 1 sec
void flash() {
	//decrement counter 40 for 200 ms
	while(1) {
		PORTF |= 0x0f;  //turn lights on
		delay(200);		//turn lights off
		PORTF &= 0xf0;
		if(checkButton()) {
			return;
		}
		delay(1000);
		if(checkButton()) {
			return;
		}
	}
}

int checkButton() {
	//PORTF |= 1;
	if(button0pressed || button1pressed) {
		if(button0pressed) {
			button0pressed = 0;
			state = stateTable[0][state];
		} else {
			button1pressed = 0;
			state = stateTable[1][state];
		}
		return 1;
	}
	return 0;
}

void delay(unsigned int msec) {
	
	unsigned int count = msec / 5;
	int done = 0;
	
	while(!done) {
		while((count > 0) && (!(button0pressed || button1pressed))) {
			_delay_ms(5);
			count--;	
		}
		if(count == 0 || button0pressed || button1pressed)
			done = 1;
	}
}