/*#################################################################################################
	Für		: Auslesen DS1621 / DS1631
	Version	: 1.0
	autor	: Martin Junghans	info@jtronics.de   www.jtronics.de
	License	: GNU General Public License 
	Hardware: ATmega128, 16MHz,
	Aufgabe	: 
//################################################################################################*/

//#################################### check CPU Takt
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//#################################### include
#include <stdint.h>
#include <avr/io.h>

#include "macros.h"
#include "ds1621.h"
#include "ds1621.h"
#include "i2cmaster.h"

//########################################################################################## 
void ds1621_init(uint8_t addr) 
{
	i2c_start_wait(addr+I2C_WRITE);
	i2c_write( CMD_START_CONVERT );			
	i2c_stop();  
}
//########################################################################################## Register auslesen
uint8_t ds1621_getreg(uint8_t addr, uint8_t cmd) 	
{
	uint8_t data;

	i2c_start_wait(addr+I2C_WRITE);  		
	i2c_write(cmd);
	i2c_stop();
	
	i2c_rep_start(addr+I2C_READ);  			
	data = i2c_read(0);
	i2c_stop();

	return data;
}
//########################################################################################## Temp auslesen --> 10,5
int16_t ds1621_get_temp(uint8_t addr) 			// (z.B. 105^=10,5°C)
{	
	if (i2c_start(addr+I2C_WRITE)) 		
	{
		i2c_stop();				
		return -9999;	// Error		
	}

	else        		
	{
		int		temp = -9999;		
		uint8_t temp_msb;
		uint8_t temp_lsb;		
		i2c_write( CMD_READ_TEMP );				// auffordern zum auslesen
		i2c_stop();  
		
		i2c_rep_start(addr+I2C_READ);  			
		temp_msb = i2c_read(1);					// read the first temperature byte (msb)
		temp_lsb = i2c_read(0);					// read the first temperature byte (lsb)
		i2c_stop();
		
		temp = temp_msb*10;
	
		if (temp_msb & 	0x80)	{ temp = (128 - (temp_msb & 0x7F))*(-10);}
		if (temp_lsb ==	0x80) 	{ temp=temp+5;	}		
		return temp;			
	}
}
//########################################################################################## Temp auslesen --> 10,5123
int16_t ds1621_get_temp_h(uint8_t addr) 			// (z.B. 1052^=10,52°C)
{

	if (i2c_start(addr+I2C_WRITE)) 		// failed to issue start condition, possibly no device found
	{
		i2c_stop();	
		return -9999;	// Error		
	}
	
	else        		// issuing start condition ok, device accessible
	{
		int		temp = -9999;		
		uint8_t temp_msb;
		uint8_t temp_lsb;
			
		i2c_write( CMD_READ_TEMP );			 	// auffordern zum auslesen
		i2c_stop();                         	// stop

		i2c_rep_start(addr+I2C_READ);  			// set device address
		temp_msb = i2c_read(1);					// read the first temperature byte (msb)
		temp_lsb = i2c_read(0);					// read and discard the 9th (i.e. 0.5C) bit
		i2c_stop();

		uint8_t reg_count = ds1621_getreg(addr, CMD_READ_COUNTER);
		uint8_t reg_slope = ds1621_getreg(addr, CMD_READ_SLOPE);

		temp = (float)temp_msb*100-25+((float)reg_slope-(float)reg_count)/(float)reg_slope*100;
		return temp;		
	}	
}
//########################################################################################## Obere Thermostatgrenze setzen
void ds1621_sethigh(uint8_t addr, int16_t temp)	
{
	uint8_t	data1, 
			data2;
			
	if (temp <0)	{ data1=255+temp/10;	}
	else 			{ data1=temp/10;		}
	
	data2=((temp % 10) >= 5) & 0x80;
	
	i2c_start_wait(addr+I2C_WRITE); 		
	i2c_write(0xA1);
	i2c_write(data1);
	i2c_write(data2);
	i2c_stop();	
}
//########################################################################################## Untere Thermostatgrenze setzen
void ds1621_setlow(uint8_t addr, int16_t temp)		
{
	uint8_t	data1, 
			data2;
			
	if (temp <0)	{ data1=255+temp/10;	}
	else 			{ data1=temp/10;		}
	data2=((temp % 10) >= 5) & 0x80;
	
	i2c_start_wait(addr+I2C_WRITE);		
	i2c_write(0xA2);
	i2c_write(data1);
	i2c_write(data2);
	i2c_stop();
}
//##########################################################################################
