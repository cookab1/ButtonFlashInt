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
		if(state != 0)
		return;
	}
}

//rotate the lights being on from 0 - 3 each in turn
void rotate() {
	while(1) {
		PORTF |= 0x1;
		delay(1000);
		PORTF &= 0xf0;
		if(state != 1)
		return;
		
		PORTF |= 0x2;
		delay(1000);
		PORTF &= 0xf0;
		if(state != 1)
		return;
		
		PORTF |= 0x4;
		delay(1000);
		PORTF &= 0xf0;
		if(state != 1)
		return;
		
		PORTF |= 0x8;
		delay(1000);
		PORTF &= 0xf0;
		if(state != 1)
		return;
	}
}

// flash the lights for 200 mSec and off for 1 sec
void flash() {
	//decrement counter 40 for 200 ms
	while(1) {
		PORTF |= 0x0f;
		delay(200);
		if(state != 2) {
			PORTF &= 0xf0;
			return;
		}
		PORTF &= 0xf0;
		delay(1000);
		if(state != 2) {
			return;
		}
	}
}

void delay(unsigned int msec) {
	
	int pins;
	unsigned int count = msec / 5;
	int done = 0;
	int bothPressed = 0;
	
	while(!done) {
		pins = (PINF >> 6) & 0x3;
		switch (pins) {
			case 0x3: //no buttons pressed
			while((count > 0) && (0x3 == ((PINF >> 6) & 0x3))) {
				_delay_ms(5);
				count--;
			}
			break;
			
			case 0x2: //button0 pressed
			while((count > 0) && (0x2 == ((PINF >> 6) & 0x3))) {
				_delay_ms(5);
				count--;
			}
			if((0x3 == ((PINF >> 6) & 0x3)) || (bothPressed && (0x2 == ((PINF >> 6) & 0x3)))) { //button0 released
				if(bothPressed)
				state = stateTable[1][state];
				else
				state = stateTable[0][state];
				done = 1;
			}
			break;
			
			case 0x1: //button1 pressed
			for(; ((count > 0) && (0x1 == ((PINF >> 6) & 0x3))); count--) {
				_delay_ms(5);
			}
			if((0x3 == ((PINF >> 6) & 0x3)) || (bothPressed && (0x1 == ((PINF >> 6) & 0x3)))) { //button1 released
				if(bothPressed)
				state = stateTable[0][state];
				else
				state = stateTable[1][state];
				done = 1;
			}
			break;
			
			case 0x0: //both buttons pressed
			bothPressed = 1;
			for(; ((count > 0) && (0x0 == ((PINF >> 6) & 0x3))); count--) {
				_delay_ms(5);
			}
			break;
		}
		if(count == 0)
		done = 1;
	}
}