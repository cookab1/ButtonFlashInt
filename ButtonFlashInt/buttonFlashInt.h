/*
 * buttonFlash.h
 *
 * Created: 2/21/2018 9:45:37 PM
 *  Author: Andy
 */ 


#ifndef BUTTONFLASH_H_
#define BUTTONFLASH_H_

static int state;
volatile int button0pressed;
volatile int button1pressed;
volatile int buttonPressed;
volatile int bothPressed;

void off();
void rotate();
void flash();

void delay(unsigned int);
int checkButton();


#endif /* BUTTONFLASH_H_ */