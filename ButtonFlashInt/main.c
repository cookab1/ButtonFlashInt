/*
 * ButtonFlash.c
 *
 * Created: 2/18/2018 10:11:41 PM
 * Author : Andy
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //sei() = set | cli() = clear
#include "buttonFlashInt.h"


int main(void)
{
	sei(); //set global interrupt enable
	DDRF = 0x0f;
	DDRK = 0x0;
	PCMSK2 = 0x3;
	
	PORTK = 0x3;
	PCICR = 0x4;
	PORTF &= 0x0;
	
	state = 0;
	button0pressed = 0;
	button1pressed = 0;
	buttonPressed = 0;
	bothPressed = 0;
	
    while (1) 
    {
	    switch(state) {
			case 0:
				off();
				break;
		    case 1:
				rotate();
				break;
		    case 2:
				flash();
				break;
	    }
    }
}

ISR(PCINT2_vect) {
	if(bothPressed) {
		bothPressed = 0;
		switch(PINK & 0x3) {
			case 0x3:			//button released
				if(button0) {
					button0 = 0;
					button0pressed = 1;
				} else { //button1 was pressed
					button1 = 0;
					button1pressed = 1;
				}
			break;
			case 0x2:			//button1 released
				button0 = 1;
				break;
			case 0x1:			//button0 released
				button1 = 1;
				break;
		}
	} else {
		switch(PINK & 0x3) {
			case 0x3:			//button released
				if(button0) {
					button0 = 0;
					button0pressed = 1;
				} else { //button1 was pressed
					button1 = 0;
					button1pressed = 1;
				}
				break;
			case 0x2:			//button0 pressed
				button0 = 1;
				break;
			case 0x1:			//button1 pressed
				button1 = 1;
				break;
			case 0x0:			//both are pressed
				bothPressed = 1;
				button1 = 1;
				button0 = 1;
				break;
		}
			
		buttonPressed = 1;
		else if(((PINK & 0x3) == 0x3) && buttonPressed) {
			buttonPressed = 0;
			if((PORTF & 0xf) == 0x0)
			PORTF |= 0x3; //turn right lights on
			else
			PORTF &= 0x0; //turn lights off
		}		
	}
	
	
	switch (PINK & 0x3) {
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