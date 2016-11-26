/*
 * button.h
 *
 * Created: 10/19/2016 10:03:54 PM
 *  Author: Hanst
 */ 

#ifndef BUTTON_H
#define BUTTON_H

#define NUM_BUTTONS 1
#define BUTTON_IN PINC //Uses pins 0 to NUM_BUTTONS-1 as inputs

char but_i = 0;

enum ButtonState {INIT_B,ON_B,OFF_B,PRESS_B} button_state[NUM_BUTTONS];

void Button_Init(){
	for (but_i = 0;but_i< NUM_BUTTONS; but_i++)
		button_state[but_i] = INIT_B;
}

void Button_Tick(char button[NUM_BUTTONS]){
	for (but_i = 0;but_i< NUM_BUTTONS; but_i++) {
		char A = ~BUTTON_IN & (0x01<<but_i);
		switch (button_state[but_i]){
			case INIT_B:
			break;
			case ON_B:
			button[but_i] = 1;
			case OFF_B:
			break;
			case PRESS_B:
			break;
			default:
			break;
		}
		switch (button_state[but_i]){
			case INIT_B:
			button_state[but_i] = OFF_B;
			break;
			case ON_B:
			button_state[but_i] = PRESS_B;
			break;
			case OFF_B:
			if (A) button_state[but_i] = ON_B;
			break;
			case PRESS_B:
			if (!A) button_state[but_i] = OFF_B;
			break;
			default:
			button_state[but_i] = INIT_B;
			break;
		}
	}
}

#endif BUTTON_H