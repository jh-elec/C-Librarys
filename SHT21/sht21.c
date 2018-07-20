

#include <stdint.h>

#include "sht21.h"
#include "I2C.h"
#include "../error.h"


static uint8_t sht21CalcCrc(uint8_t *data,uint8_t nbrOfBytes);
uint8_t sht21Reads( uint8_t *buff , uint8_t leng );
uint8_t sht21Writes( uint8_t *buff , uint8_t leng );
uint8_t sht21Reads_( uint8_t *buff , uint8_t leng );


uint8_t sht21Read(int16_t *temp, uint16_t *humidity)
{
	uint8_t d[3];
	uint32_t val;
	
	//=== Software reset =============================================
 	d[0] = CMD_SOFT_RST;
 	sht21Writes( d , 1 );


	//=== User register ========================================================
	d[0] = CMD_RD_REG;
	sht21Reads( d , 2 );

	if(d[1] == sht21CalcCrc(d,1))
	{
		uint8_t buff[] = { CMD_WR_REG , d[0] };
		sht21Writes( buff , 2 );
	}


	//=== Temperature ===========================================================
	d[0] = CMD_TMP_HLD;
	sht21Reads( d , 3 );
	
	if(d[2] == sht21CalcCrc(d,2))
	{
		val = d[0];
		val <<= 8;
		val += d[1];
		val &=  ~(0x0003);
		*temp = -46.85 + 175.72/65536 *(float)val;
	}
	
	//=== Humidity ==============================================================
	d[0] = CMD_HUM_HLD;
	sht21Reads( d , 3 );
	
	if(d[2] == sht21CalcCrc(d,2))
	{
		val = d[0];
		val <<= 8;
		val += d[1];
		val &= ~0x0003;
		*humidity = 6.0 + 125.0/65536 * (float)val;
	}
	
	return(0);
}

uint8_t sht21GetSerialNumber( uint8_t numb[8] )
{
	uint8_t d[8] = "";
	d[0] = 0xFA;
	d[1] = 0x0F;
	sht21Writes( d , 2 );

	sht21Reads_( d , 8 );
	numb[5] = d[0];
	numb[4] = d[2];
	numb[3] = d[4];
	numb[2] = d[6];
	
	d[0] = 0xFC;
	d[1] = 0xC9;
	sht21Writes( d , 2 );
	
	sht21Reads_( d ,  6 );
	numb[1] = d[0];
	numb[0] = d[1];
	numb[7] = d[3];
	numb[6] = d[4];

	return 0;
}

static uint8_t sht21CalcCrc(uint8_t *data,uint8_t nbrOfBytes)
{
	// CRC
	//const u16t POLYNOMIAL = 0x131; //P(x)=x^8+x^5+x^4+1 = 100110001
	
	uint8_t byteCtr,bit,crc;
	
	crc = 0;
	
	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
	{
		crc ^= (data[byteCtr]);
		for (bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80) crc = (crc << 1) ^ 0x131;
			else 		crc = (crc << 1);
		}
	}
	return(crc);
}

uint8_t sht21Reads( uint8_t *buff , uint8_t leng )
{
	i2c_start_wait( SHT21_ADDR + I2C_WRITE );
	
	if( i2c_write( buff[0] ) ) // Register Adresse
	{
		errorWriteCircular( &err , _ERROR_SHT21_I2C_ , ERROR_I2C_ADDRESS_TX );
	}
	
	if ( i2c_rep_start( SHT21_ADDR + I2C_READ ) )
	{
		errorWriteCircular( &err , _ERROR_SHT21_I2C_ , ERROR_I2C_NO_ACK );		
		i2c_stop();
		return 1;
	}
	
	for ( uint8_t i = 0 ; i < leng ; i++ )
	{
		if ( leng == 1 )
		{
			*buff = i2c_readNak();
			return 0;	
		}
		else
		{
			*buff++ = i2c_readAck();	
		}	
	}
	*buff = i2c_readNak();
	i2c_stop();
	
	return 0;
}

uint8_t sht21Writes( uint8_t *buff , uint8_t leng )
{
	i2c_start_wait( SHT21_ADDR + I2C_WRITE );

	for ( uint8_t i = 0 ; i < leng ; i++ )
	{
		if ( leng == 1 )
		{
			if ( i2c_write( *buff ) )
			{
				errorWriteCircular( &err , _ERROR_SHT21_I2C_ , ERROR_I2C_BYTE_TX );
			}
			
			i2c_stop();
			return 0;
		}
		else
		{
			if ( i2c_write( *buff++ ) )
			{
				errorWriteCircular( &err , _ERROR_SHT21_I2C_ , ERROR_I2C_BYTE_TX );				
			}
			
		}
	}
	i2c_stop();
	
	return 0;
}

uint8_t sht21Reads_( uint8_t *buff , uint8_t leng )
{
	i2c_start_wait( SHT21_ADDR + I2C_READ );
	for ( uint8_t i = 0 ; i < leng ; i++ )
	{
		if ( leng == 1 )
		{
			*buff = i2c_readNak();
			return 0;
		}
		else
		{
			*buff++ = i2c_readAck();
		}
	}
	*buff = i2c_readNak();
	i2c_stop();
	
	return 0;
}
