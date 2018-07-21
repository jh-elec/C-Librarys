/*
 * Copyright:		Jan Homann
 * Author:			Jan Homann
 * Version:			1.1
 * Last Change :	07.04.2016
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif


#include <avr/io.h>
#include <util/delay.h>
#include "md09a.h"

void md09a_init(void)
{
	STEP_SLEEP_DDR	|= (1<<STEP_SLEEP_PIN);	
	STEP_PULS_DDR	|= (1<<STEP_PULS_PIN);
	STEP_DIR_DDR	|= (1<<STEP_DIR_PIN);
	STEP_EN_DDR		|= (1<<STEP_EN_PIN);
	STEP_MS1_DDR	|= (1<<STEP_MS1_PIN);
	STEP_MS2_DDR	|= (1<<STEP_MS2_PIN);
	STEP_MS3_DDR	|= (1<<STEP_MS3_PIN);
}

uint8_t md09a_command(uint16_t steps, uint8_t direction , uint8_t step_type , uint8_t sleep)
{	
	STEP_SLEEP_PORT |= (1<<STEP_SLEEP_PIN);
	
	if (direction == LEFT)
	{
		STEP_DIR_PORT |= (1<<STEP_DIR_PIN); // RIGHT or LEFT?
	}
	else if(direction == RIGHT)
	{
		STEP_DIR_PORT &= ~(1<<STEP_DIR_PIN); // RIGHT or LEFT?
	}
	else if (direction != (RIGHT || LEFT))
	{
		return 1; // direction error
	}
	
	switch(step_type)
	{
		case STEP_FULL: 
		{			
			STEP_MS1_PORT &= ~(1<<STEP_MS1_PIN);
			STEP_MS2_PORT &= ~(1<<STEP_MS2_PIN);
			STEP_MS3_PORT &= ~(1<<STEP_MS3_PIN);
			
		}break;
		
		case STEP_HALF: 
		{
			STEP_MS1_PORT |= (1<<STEP_MS1_PIN);
			STEP_MS2_PORT &= ~(1<<STEP_MS2_PIN);
			STEP_MS3_PORT &= ~(1<<STEP_MS3_PIN);
				
		}break;
		
		case STEP_QUAD: 
		{
			STEP_MS1_PORT &= ~(1<<STEP_MS1_PIN);
			STEP_MS2_PORT |=  (1<<STEP_MS2_PIN);
			STEP_MS3_PORT &= ~(1<<STEP_MS3_PIN);
			
		}break;
		
		case STEP_EIGH: 
		{			
			STEP_MS1_PORT |=  (1<<STEP_MS1_PIN);
			STEP_MS2_PORT |=  (1<<STEP_MS2_PIN);
			STEP_MS3_PORT &= ~(1<<STEP_MS3_PIN);
			
		}break;
		
		case STEP_SIXT: 
		{	
			STEP_MS1_PORT |= (1<<STEP_MS1_PIN);
			STEP_MS2_PORT |= (1<<STEP_MS2_PIN);
			STEP_MS3_PORT |= (1<<STEP_MS3_PIN);
				
		}break;
		
		default : 
		{
			return 2; // step type error
		}break;
	}
	
	for (uint16_t x = 0 ; x < steps ; x++ )
	{
		STEP_PULS_PORT &= ~(1<<STEP_PULS_PIN);
		_delay_us(500);
		STEP_PULS_PORT |= (1<<STEP_PULS_PIN);
		_delay_us(500);
	}	

	
	if (sleep == DISABLE)
	{
		STEP_SLEEP_PORT |= (1<<STEP_SLEEP_PIN);
		STEP_EN_PORT	&= ~(1<<STEP_EN_PIN);
	}
	else if (sleep == ENABLE)
	{
		STEP_SLEEP_PORT &= ~(1<<STEP_SLEEP_PIN);
		STEP_EN_PORT	|=  (1<<STEP_EN_PIN);
	}
	else if (sleep != (ENABLE || DISABLE || IGNORE))
	{
		return 3; // sleep error
	}
	
	return 0; // command successfully 
}