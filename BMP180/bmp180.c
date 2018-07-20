/*
 * Copyright:      Jan Homann
 * Author:         Jan Homann
 * Version:        1.0
 */


#define F_CPU 16000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "BMP180.h"
#include "i2cmaster.h"
#include "Super_Clokk.h"


bmp180_t cal;

char bmp180GetCal(bmp180_t *cal)
{
	char ret = 1;
	
	bmp180ReadInt(0xAA, &(cal->AC1));
	bmp180ReadInt(0xAC, &(cal->AC2));
	bmp180ReadInt(0xAE, &(cal->AC3));
	bmp180ReadUint(0xB0, &(cal->AC4));
	bmp180ReadUint(0xB2, &(cal->AC5));
	bmp180ReadUint(0xB4, &(cal->AC6));
	bmp180ReadInt(0xB6, &(cal->B1));
	bmp180ReadInt(0xB8, &(cal->B2));
	bmp180ReadInt(0xBA, &(cal->MB));
	bmp180ReadInt(0xBC, &(cal->MC));
	bmp180ReadInt(0xBE, &(cal->MD));
	
	return ret;
}

char bmpGetID(char *id)
{
	return bmp180ReadByte(0xD0, id);
}

char bmpGetTemperaure(bmp180_t cal, long *temperatur)
{
	char ret = 1;
	int t;
	long UT, X1, X2, B5, T;
	long AC5, AC6, MC, MD;
	AC5 = cal.AC5;
	AC6 = cal.AC6;
	MC = cal.MC;
	MD = cal.MD;
	ret &= bmp180StartTemperature(&t);
	UT = (unsigned int)t;
	X1 = (((UT - AC6) * AC5)>>15);
	X2 = (MC<<11) / (X1 + MD);
	B5 = X1 + X2;
	T =((B5 + 8L)>>4);
	*temperatur = T;
	return ret;
}

char bmp180GetPressure(bmp180_t *cal, unsigned char oss )
{
	char ret = 1;
	int t;
	long UT, X1, X2, X3, B3, B5, B6, T, AC1, AC2, AC3, AC4, AC5, AC6, MC, MD, B1, B2 , p;
	unsigned long B4, B7, UP;
	
	AC1		= cal->AC1;
	AC2		= cal->AC2;
	AC3		= cal->AC3;
	AC4		= cal->AC4;
	AC5		= cal->AC5;
	AC6		= cal->AC6;
	MC		= cal->MC;
	MD		= cal->MD;
	B1		= cal->B1;
	B2		= cal->B2;
		
	ret &= bmp180StartTemperature(&t);
	UT = (long)t;
	ret &= bmp180StartPressure(&p, oss);
	UP = (long)p;

	X1 = ( ( ( UT - AC6 ) * AC5 ) >> 15 );
	X2 = (MC<<11) / (X1 + MD);
	B5 = X1 + X2;
	T = ((B5 + 8L)>>4);
	cal->temperature = T;
	
    B6 = B5 - 4000;	
    X1 = (B2 * (B6 * B6 >> 12)) >> 11;
    X2 = AC2 * B6 >> 11;	
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3) << oss) + 2) >> 2;
    X1 = AC3 * B6 / 8192;
    X2 = (B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = AC4 * (unsigned long)(X3 + 32768) >> 15;
    B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	
    if (B7 < 0x80000000)
	{
		p = (B7 * 2) / B4;
	}
    else
	{
		p = (B7 / B4) * 2;
	}
    
	
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    cal->pressure = p + ((X1 + X2 + 3791) >> 4);
	
    return ret;
}

char bmp180StartMeasure(char address, char command, char statusbit)
{
	char status = command;
	unsigned int timeout = 65535;
	if (bmp180WriteByte(address, command))
	{
		while((status & statusbit) && (timeout-- > 0))
		{
			if (!bmp180ReadByte(address, &status)) return 0;
		}
		return 1;
	}
	return 0;
}

char bmp180StartTemperature(int *value)
{
	int hi;
	if (bmp180StartMeasure(0xF4, 0x2E, 0x00 | (1<<BMP180_SCO)))
	{
		bmp180ReadInt(0xF6, &hi);
		*value = (int)hi;
		return 1;
	}
	return 0;
}

char bmp180StartPressure(long *value, unsigned char oss)
{
	char hi, lo;
	char xlsb;
	if (bmp180StartMeasure(0xF4, 0x34 + (oss<<6), 0x00 | (1<<BMP180_SCO)))
	{
		bmp180ReadByte(0xF6, &hi);
		bmp180ReadByte(0xF7, &lo);
		bmp180ReadByte(0xF8, &xlsb);
		*value = (long)((((long)hi)<<16) | (((long)lo)<<8) | ((long)xlsb)) >> (8-oss);
		return 1;
	}
	return 0;
}

char bmp180ReadByte(char address, char *value)
{
	unsigned char data[1];

	data[0] = address;
	if (bmp180ReadBytes(data,1))
	{
		*value = data[0];
		return 1;
	}
	value = 0;
	return 0;
}

char bmp180ReadUByte(char address, unsigned char *value)
{
	unsigned char data[1];

	data[0] = address;
	
	if (bmp180ReadBytes(data,1))
	{
		*value = data[0];
		return 1;
	}
	
	value = 0;
	
	return 0;
}

char bmp180ReadInt(char address, int *value)
{
	unsigned char data[2];

	data[0] = address;
	
	if (bmp180ReadBytes(data,2))
	{
		*value = (((int)data[0]<<8)|(int)data[1]);
		return 1;
	}
	
	value = 0;
	
	return 0;
}

char bmp180ReadUint(char address, unsigned int *value)
{
	unsigned char data[2];

	data[0] = address;
	
	if (bmp180ReadBytes(data,2))
	{
		*value = (((unsigned int)data[0]<<8)|(unsigned int)data[1]);
		return 0;
	}
	
	value = 0;
	
	return 1;
}

char bmp180WriteByte(char address, char value)
{
	unsigned char data[2];

	data[0] = address;
	data[1] = value;
	if (bmp180WriteBytes(data,1))
	{
		return 1;
	}
	value = 0;
	return 0;
}

char bmp180ReadBytes(unsigned char *values, char length)
{
	unsigned char addr = values[0];
	
	if ( length == 1)
	{
		i2c_start_wait( BMP180 + I2C_WRITE );
		i2c_write( addr );
		i2c_stop( );
		
		i2c_start_wait( BMP180 + I2C_READ );
		*values = i2c_readNak( );
		i2c_stop( );				
	}
	else
	{
		for ( uint8_t i = 0 ; i < length ; i++ )
		{
			i2c_start_wait( BMP180 + I2C_WRITE );
			i2c_write( addr + i );        
			i2c_stop( );                          

			i2c_start_wait( BMP180 + I2C_READ );    
			*values++ = i2c_readNak( );         
			i2c_stop( );    		
		}		
	}
	
	return 1;
}

char bmp180WriteBytes(unsigned char *values, char length)
{
	unsigned char ret, res = 0, zc = 0;
	unsigned char addr = values[0];

	while(zc < length)
	{
		ret = i2c_start(BMP180+I2C_WRITE);        // ret=0 -> Ok, ret=1 -> no ACK
		if (!ret)
		{
			ret = i2c_write(addr + zc);                       // write Instruction Byte
			res = res + ret;
			ret = i2c_write(values[zc + 1]);                       // Date Byte
			_delay_ms(150);
			res = res + ret;
			i2c_stop();                                // set stop conditon = release bus
		}
		res = res + ret;
		if(res) return 0;
		zc++;
	}
	return 1;
}

void bmp180Init(void)
{
	bmp180GetCal(&cal);
}