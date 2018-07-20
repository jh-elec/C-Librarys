/*
*	made by		: Jan Homann (C)
*	Copyright	: Jan Homann (C)
*	Date		: 20.05.2015
*/


#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "HEF4094.h"



void HEF4094_init(void)
{
	HEF4094_DATA_DDR	|=	(1<<HEF4094_DATA_PIN);
	HEF4094_CLOCK_DDR	|=	(1<<HEF4094_CLOCK_PIN);
	HEF4094_STROBE_DDR	|=	(1<<HEF4094_STROBE_PIN);
}

void HEF4094_send_Data(uint8_t mask)
{
	
		HEF4094_STROBE_PORT &= ~(1<<HEF4094_STROBE_PIN);// strobe low
			
		for (uint8_t x = 0; x < 8 ; x++)
		{
			if (mask & 0x80)
			{
				HEF4094_DATA_PORT |= (1<<HEF4094_DATA_PIN);// data high
			}
			else
			{
				HEF4094_DATA_PORT &= ~(1<<HEF4094_DATA_PIN);// data low
			}
			
			HEF4094_CLOCK_PORT &= ~(1<<HEF4094_CLOCK_PIN);// clock low	
			HEF4094_CLOCK_PORT  |=  (1<<HEF4094_CLOCK_PIN);// clock high
		
			mask <<= 1;

		}// end for
		
		
		HEF4094_STROBE_PORT |= (1<<HEF4094_STROBE_PIN);// strobe high		
		HEF4094_STROBE_PORT &= ~(1<<HEF4094_STROBE_PIN);// strobe low		
}