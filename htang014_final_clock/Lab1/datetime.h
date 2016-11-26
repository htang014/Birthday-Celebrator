/*
 * datetime.h
 *
 * Created: 11/25/2016 8:43:18 PM
 *  Author: Hanst
 */ 


#ifndef DATETIME_H_
#define DATETIME_H_

#include "usart_ATmega1284.h"

unsigned char sec = 0, min = 0, hour = 0;
unsigned char dday = 0, dmonth = 0, dyear = 0;

char daysInMonth(char month, char year){
	if (month == 1 || month == 3 || month == 5 || month == 7 ||
		month == 8 || month == 10 || month == 12){
			return 31;
		}
	else if (month == 2){
		if (year%4)
			return 28;
		else
			return 29;	
	}
	else
		return 30;
}

char sendTime(char* str){
	if (str != "\0"){
		USART_Flush(0);
		char hourStr[3];
		char minStr[3];
		char secStr[3];
		memcpy(hourStr, str, 2);
		memcpy(minStr, &str[2], 2);
		memcpy(secStr, &str[4], 2);
		hourStr[2] = '\0';
		minStr[2] = '\0';
		secStr[2] = '\0';

		if (atoi(hourStr) < 24 && atoi(minStr) < 60 && atoi(secStr) < 60){
			hour = atoi(hourStr);
			min = atoi(minStr);
			sec = atoi(secStr);
			
			while (!USART_IsSendReady(1));
			USART_Send(0xAA,1);
			while (!USART_IsSendReady(1));
			USART_Send(hour,1);
			while (!USART_HasTransmitted(1));
			while (!USART_IsSendReady(1));
			USART_Send(min,1);
			while (!USART_HasTransmitted(1));
			while (!USART_IsSendReady(1));
			USART_Send(sec,1);
			while (!USART_HasTransmitted(1));
		}
		else
			return 0;
			
		return 1;
	}
	
	return 0;
}

char sendDate(char* str){
	if (str != "\0"){
		USART_Flush(0);
		char yearStr[3];
		char monthStr[3];
		char dayStr[3];
		memcpy(monthStr, str, 2);
		memcpy(dayStr, &str[2], 2);
		memcpy(yearStr, &str[4], 2);
		monthStr[2] = '\0';
		dayStr[2] = '\0';
		yearStr[2] = '\0';
		
		char dayChar = atoi(dayStr);
		char monthChar = atoi(monthStr);
		char yearChar = atoi(yearStr);
		
		if (!dayChar || !monthChar || !yearChar)
			return 0;

		if (yearChar > 99 && monthChar > 12 && dayChar > daysInMonth(monthChar, yearChar))
			return 0;
		
		dday = atoi(dayStr);
		dmonth = atoi(monthStr);
		dyear = atoi(yearStr);
		
		while (!USART_IsSendReady(1));
		USART_Send(0xFF,1);
		while (!USART_IsSendReady(1));
		USART_Send(dday,1);
		while (!USART_HasTransmitted(1));
		while (!USART_IsSendReady(1));
		USART_Send(dmonth,1);
		while (!USART_HasTransmitted(1));
		while (!USART_IsSendReady(1));
		USART_Send(dyear,1);
		while (!USART_HasTransmitted(1));
			
		return 1;
	}
	
	return 0;
}

#endif /* DATETIME_H_ */