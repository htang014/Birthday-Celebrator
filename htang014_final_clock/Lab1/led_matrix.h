//TO BE USED WITH SR_TRANSMIT.H.

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "sr_transmit.h"

#define LED_ROW PORTD
#define DDR_ROW DDRD

void led_init()
{
	sr_init();
	DDR_ROW = 0xFF;
}

void led_display(char row, char col)
{
	transmit_data(col);
	LED_ROW = ~row; 
}

#endif