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
	if((PINK & 0x3) == 0x2) //if button 0 was pressed // || ((PINK & 0x3) == 0x1)
		buttonPressed = 1;
	else if(((PINK & 0x3) == 0x3) && buttonPressed) {
		buttonPressed = 1;
		PORTF &= 0xf; //turn lights on
	}
}

