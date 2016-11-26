/*
 * stepper.h
 *
 * Created: 11/6/2016 4:02:53 PM
 *  Author: Hanst
 */ 
#ifndef STEPPER_H_
#define STEPPER_H_

#include "bit.h"

#define STEP_PORT PORTC
#define PHASES_90 (90 / 5.625) * 64
#define PHASES_180 (180 / 5.625) * 64
#define PHASES_360 (360 / 5.625) * 64

char stepper_state = 0;

void stepper_update (){
	char x = GetBit(stepper_state, 2),
		 y = GetBit(stepper_state, 1),
		 z = GetBit(stepper_state, 0);
		 
	STEP_PORT = 0;
		 
	if ( (!x && !y) || (x && y && z) )
		STEP_PORT = SetBit( STEP_PORT, 7, 1);
	if ( !x && (y || (!y && z)) )
		STEP_PORT = SetBit( STEP_PORT, 6, 1);
	if ( (x && !y) || (!x && y && z) )
		STEP_PORT = SetBit( STEP_PORT, 5, 1);
	if ( x && (y || (!y && z)) )
		STEP_PORT = SetBit( STEP_PORT, 4, 1);
}

void stepper_cw() {
	stepper_update();
			
	stepper_state++;
	if (stepper_state >= 8)
	stepper_state = 0;
}

void stepper_ccw (){
	stepper_update();
	
	stepper_state--;
	if (stepper_state < 0)
	stepper_state = 7;
}

#endif /* STEPPER_H_ */