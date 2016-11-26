/*
 * IncFile1.h
 *
 * Created: 11/12/2016 3:27:05 AM
 *  Author: Hanst
 */ 


#ifndef DELAY_H_
#define DELAY_H_

void delay_ms(int miliSec) { //for 8 Mhz crystal
	int i,j;
	for(i=0;i<miliSec;i++) {
		for(j=0;j<775;j++) {
			asm("nop");
		}
	}
}

#endif /* DELAY_H_ */