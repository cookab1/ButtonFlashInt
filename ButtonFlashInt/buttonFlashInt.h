/*
 * buttonFlash.h
 *
 * Created: 2/21/2018 9:45:37 PM
 *  Author: Andy
 */ 


#ifndef BUTTONFLASH_H_
#define BUTTONFLASH_H_

int state;
int button;

void off();
void rotate();
void flash();


void delay_usec(unsigned int);
void delay(unsigned int);


#endif /* BUTTONFLASH_H_ */