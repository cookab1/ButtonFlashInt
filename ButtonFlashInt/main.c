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
			case 0x3:			//both released
				button0 = 0;
				button1 = 0;
				break;
			case 0x2:			//button1 released
				button1 = 0;
				button1pressed = 1;
				break;
			case 0x1:			//button0 released
				button0 = 0;
				button0pressed = 1;
				break;
		}
	} else {
		switch(PINK & 0x3) {
			case 0x3:				//button released
				if(button0) {		//if button0 released
					button0 = 0;
					button0pressed = 1;
				} else {			//else button1 was released
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
	}
}