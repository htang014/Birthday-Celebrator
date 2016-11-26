/*
 * serialRF.c
 *
 * Created: 11/21/2port16 2:2port:port8 PM
 *  Author: Hanst
 */ 
#include "check.h"
#include "delay.h"
#include "usart_ATmega1284.h"

void rf_send(char * message, char length, char port){
		while (!USART_IsSendReady(port));
		USART_Send(0xAA, port);
		while (!USART_HasTransmitted(port));
		
		while (!USART_IsSendReady(port));
		USART_Send(length, port);
		while (!USART_HasTransmitted(port));

		for (uint8_t i = 0; i < length; i++){
			while (!USART_IsSendReady(port));
			USART_Send(message[i], port);
			while (!USART_HasTransmitted(port));
		}
		
		while (!USART_IsSendReady(port));
		USART_Send(checksum(message, length), port);
		while (!USART_HasTransmitted(port));

}