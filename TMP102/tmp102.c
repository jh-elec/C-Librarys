/* Generated by CodeDescriptor 1.3.0.0714 */
/*
* Project Name      -> TMP102
* Version           -> 1.0.0.0816
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 16.08.2017 06:29:22
* Description       -> Description
*
*
*
*/

#include <avr/io.h>
#include "tmp102.h"
#include "i2cmaster.h"

tmp102_t tmp102 =
{
	.lowest  =	127,
	.actual  =	25,
	.highest = -127,
};


int16_t tmp102Calc		( uint16_t temp )
{
	double stemp = temp;
	stemp *= 0.0625;
	
	return (int16_t)stemp;
}

int16_t tmp102Read		( void )
{
	uint8_t tmp[] = { 0 , 0 };
	
	i2c_start_wait( TMP102_ADDR+I2C_READ );
	tmp[TMP102_MSB] = i2c_readAck();
	tmp[TMP102_LSB] = i2c_readNak();
	i2c_stop();
	
	uint16_t temp = ( (uint16_t)tmp[TMP102_MSB] << 8 | tmp[TMP102_LSB] ) >> 4;
	
	if ( temp & 1<<12 ) // Negatives Vorzeichen
	{
		return ( (int16_t)tmp[TMP102_MSB] << 8 | tmp[TMP102_LSB] ) >> 4;
	}
	else
	{
		return ( (uint16_t)tmp[TMP102_MSB] << 8 | tmp[TMP102_LSB] ) >> 4;
	}
	
	return 0;
}

int8_t tmp102GetTemp	( void )
{
	return tmp102Calc( tmp102Read() );
}