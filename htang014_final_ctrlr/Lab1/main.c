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
#include "keypad.h"
#include "delay.h"
#include "serialRF.h"

#define CODE_SIZE 4

void Demo_Init()
{
	initUSART(0);
	DDRB = 0x0F; PORTB = 0xF0;
	DDRC = 0xFF;
}

void Demo_Tick()
{	
	PORTC = 0x00;
	unsigned char keycode[20];	
	unsigned char firstKey = '\0';
	
	while (firstKey == '\0')
		firstKey = GetKeypadKeyOnce();
		
	if (firstKey == '*' || firstKey == '#'){
		for (char i = 0; i < 6; i++){
			rf_send(&firstKey, 1, 0);
		}
		
		PORTC = 0xFF;
		return;
	}

	keycode[0] = firstKey;
	GetKeypadString(&keycode[1], CODE_SIZE-1);
	keycode[CODE_SIZE] = '\0';
	
	for (char i = 0; i < 6; i++){
		rf_send(keycode, 4, 0);
		delay_ms(250);
	}
	
	PORTC = 0xFF;
}

void DemoSecTask()
{
	Demo_Init();
	for (;;)
	{
		Demo_Tick();
		vTaskDelay(250);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(DemoSecTask, (signed portCHAR *)"DemoSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
 
int main(void) 
{ 
   //Start Tasks  
   StartSecPulse(1);
   //Run Scheduler 
   vTaskStartScheduler(); 
   return 0; 
}