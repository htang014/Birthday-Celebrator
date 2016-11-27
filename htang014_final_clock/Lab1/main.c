#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

#include "lcd.h"
#include "button.h"
#include "serialRF.h"
#include "datetime.h"

#define CODE_SIZE 6

unsigned char timeDisplay[10];
unsigned char clkLCDEnable = 0;

void Clock_Init()
{
	initUSART(1);
}

void Clock_Tick()
{
	unsigned char rcvSync = 0;
	while (!USART_HasReceived(1));
	while (rcvSync != 0xAA)
		rcvSync = USART_Receive(1);
		
	while (!USART_HasReceived(1));
	hour = USART_Receive(1);
	while (!USART_HasReceived(1));
	min = USART_Receive(1);
	while (!USART_HasReceived(1));
	sec = USART_Receive(1);
	while (!USART_HasReceived(1));
	dyear = USART_Receive(1);
	while (!USART_HasReceived(1));
	dmonth = USART_Receive(1);
	while (!USART_HasReceived(1));
	dday = USART_Receive(1);
}

void ClockSecTask()
{
	Clock_Init();
	for (;;)
	{
		Clock_Tick();
		vTaskDelay(250);
	}
}

void ClockPrint_Init()
{

}

void ClockPrint_Tick()
{	
	if(clkLCDEnable){	
		sprintf(timeDisplay, "%02d:%02d:%02d", hour, min, sec);
		LCD_WriteString(1, timeDisplay);
		LCD_Cursor(0);
	}
}

void ClockPrintSecTask()
{
	ClockPrint_Init();
	for (;;)
	{
		ClockPrint_Tick();
		vTaskDelay(1000);
	}
}

char code_received = 0;
enum CodeMode {PASSWD, TIME, DATE, EVENTS} code_mode;
void Demo_Init()
{
	initUSART(0);
	LCD_init();
	LCD_ClearScreen();
	code_mode = PASSWD;
}

void Demo_Tick()
{	
	unsigned char msg[20];
		
	switch (code_mode){
		case PASSWD:
		clkLCDEnable = 1;
		
		rf_receive(msg, 0);
			
		if (msg[0] == '*'){
			clkLCDEnable = 0;
			delay_ms(1000);
			USART_Flush(0);
			code_mode = TIME;
			break;
		}
		else if (msg[0] == '#'){
			clkLCDEnable = 0;
			delay_ms(1000);
			USART_Flush(0);
			code_mode = EVENTS;
			break;
		}
		msg[CODE_SIZE] = '\0';

		char pass = 1;
		for (char i = 0; i < CODE_SIZE; i++){
			if (msg[i] > '9' || msg[i] < '0')
				pass = 0;
		}
		
		if (msg != "\0" && pass){
			LCD_WriteString(17,msg);
			delay_ms(1000);
			USART_Flush(0);	
		}
		else{
			return;
		}
		break;
			
		case TIME:
		LCD_ClearScreen();
		LCD_WriteString(1,"SET TIME");
		rf_receive(msg,0);
		
		if (msg[0] == '*'){
			clkLCDEnable = 0;
			code_mode = DATE;
			LCD_ClearScreen();
			break;
		}
		else if (msg[0] == '#'){
			clkLCDEnable = 0;
			code_mode = PASSWD;
			LCD_ClearScreen();
			break;
		}
		
		sendTime(msg);
		break;
		
		case DATE:
		LCD_ClearScreen();
		LCD_WriteString(1,"SET DATE");
		rf_receive(msg,0);
		
		if (msg[0] == '*'){
			clkLCDEnable = 0;
			code_mode = EVENTS;
			LCD_ClearScreen();
			break;
		}
		else if (msg[0] == '#'){
			clkLCDEnable = 0;
			code_mode = TIME;
			LCD_ClearScreen();
			break;
		}
		sendDate(msg);
		break;
		case EVENTS:
		code_mode = PASSWD;
		break;
	}
	code_received = 1;
}

void DemoSecTask()
{
	Demo_Init();
	for (;;)
	{
		Demo_Tick();
		vTaskDelay(100);
	}
}

char button[1];
void ButtonSecTask()
{
	Button_Init();
	for (;;)
	{
		Button_Tick(button);
		vTaskDelay(100);
	}
}

unsigned int backlightTimer = 0;
void Backlight_Tick()
{
	if (button[0]){
		backlightTimer = 1;
		button[0] = 0;
	}
	else if (backlightTimer || code_received){
		backlightTimer++;
		code_received = 0;
		PORTC |= 0x80;
	}
	
	if (backlightTimer >= 60){
		PORTC &= 0x7F;
		backlightTimer = 0;
	}
}
void BacklightSecTask()
{
	for (;;)
	{
		Backlight_Tick();
		vTaskDelay(100);
	}	
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(DemoSecTask, (signed portCHAR *)"DemoSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(ClockSecTask, (signed portCHAR *)"ClockSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(ClockPrintSecTask, (signed portCHAR *)"ClockPrintSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(ButtonSecTask, (signed portCHAR *)"ButtonSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(BacklightSecTask, (signed portCHAR *)"BacklightSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );

}
 
int main(void) 
{ 
	DDRA = 0xFF;
	DDRB = 0xFF ;
	DDRC = 0xF0; PORTC = 0x0F;
   //Start Tasks  
   StartSecPulse(1);
   //Run Scheduler 
   vTaskStartScheduler(); 	   
   return 0; 
}