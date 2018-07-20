/*
 * Copyright:    Jan Homann
 * Author:         Jan Homann
 * Version:        1.0
 * Date:		23.06.2015
 */

#include <avr/io.h>
#include <inttypes.h>
#include "74xx165.h"


void init_165A(void)
{
 	INPUT_DDR	&= ~(1<<INPUT_PIN); // set as input
// 	INPUT_PORT	|=  (1<<INPUT_PIN); // enable pullup
 	
 	LD_DDR		|=	(1<<LD_PIN); // load pin @ output
 	CLK_DDR		|=	(1<<CLK_PIN); // clk pin @ output
	 
#ifdef with_clk_inh 
 	INH_DDR		|=	(1<<INH_PIN); // clk_inh pin @ output
#endif
}

uint8_t read_SR_165A(void)
{
	uint8_t byte = 0x00;

 	LD_PORT	&= ~(1<<LD_PIN); // load parallel bits into 165A register
 	LD_PORT	|=  (1<<LD_PIN); // load disable
 
#ifdef with_clk_inh	
 	INH_PORT	&= ~(1<<INH_PIN); // clk_inh pin low
#endif
	
	for(uint8_t cnt = 0 ; cnt < 8 ; cnt ++)
	{

		
		byte <<= 0x01; // shift one position left
		
		if((INPUT_PORT & (1<<INPUT_PIN))) // read serial bit
		{
			byte |= 0x01; // set bit "1"
		}
		
		CLK_PORT		&= ~(1<<CLK_PIN);
		CLK_PORT		|=  (1<<CLK_PIN); // shift bit (rising edge)	
		
	}// end for

	return(byte); // return mask
	
}