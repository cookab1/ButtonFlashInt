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
	DDRF = 0x0f;
	DDRK = 0x3;
	
	PCICR = 0x4;
	PORTK = 0x3;
	PORTF |= 0xc0;
	
	state = 0;
	button0pressed = 0;
	button1pressed = 0;
	
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

ISR(PCINT16) {
	button0pressed = 1;
}
ISR(PCINT17) {
	button1pressed = 1;	
}

