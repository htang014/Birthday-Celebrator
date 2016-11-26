/*
 * serialRF.c
 *
 * Created: 11/21/2016 1:36:53 PM
 *  Author: Hanst
 */ 
#include "check.h"
#include "usart_ATmega1284.h"

//Receives a string from USART via RF protocol
//Returns string or #/* if valid, else null
void rf_receive(char* msg, char port){
	unsigned char rcvData = 0, chksm = 0, length = 0;

	//Dummy signal
	while (!USART_HasReceived(port));
	while (rcvData != 0xAA){
		rcvData = USART_Receive(port);
	}
	
	while (!USART_HasReceived(port));
	rcvData = USART_Receive(port);
	length = rcvData;
	
	if (length > 10)
		length = 10;
	
	//Data transfer
	for (char i = 0; i < length; i++){
		while (!USART_HasReceived(port));
		rcvData = USART_Receive(port);	
		msg[i] = rcvData;
	}
	
	//Compare checksums
	while (!USART_HasReceived(port));
	rcvData = USART_Receive(port);
	
	if (!(checksum(msg, length) == rcvData)){
		msg = 0;
	}
	
}