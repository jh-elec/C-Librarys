/*************************************************************************************
*				Driver for " DS3231 RTC "
*	Autor	: Jan Homann
*	Version : 1.0
*	Date	: 11.12.2015
***************************************************************************************/

/* for debugging */
#define UART_OUTPUT

#include <avr/io.h>
#include "i2cmaster.h"
#include "DS3231.h"

#ifdef UART_OUTPUT
#include "uart.h"
#endif


rtc ds3231 = {12,5,0,1,1,1,15};

void DS3231_set_SWOF(uint8_t clock)
{
	if (clock < 1 || clock > 4)
	{
		return;
	}
	
	i2c_start(DS3231_ADDRESS+I2C_WRITE);
	i2c_write(CONTROL);
	switch (clock)
	{
		case 1: i2c_write(0x00);break; // 1Hz
		case 2:	i2c_write(0x08);break; // 1.024kHz
		case 3:	i2c_write(0x10);break; // 4.096kHz
		case 4:	i2c_write(0x18);break; // 8.192kHz
		default: i2c_stop(); break;
	}// end switch
	i2c_stop();
}

void DS3231_get_Data(void)
{
	char time_string[8];
	char date_string[13];
	uint8_t tmp;
	
	i2c_start(DS3231_ADDRESS+I2C_WRITE);
	i2c_write(0x00);
	i2c_stop();
	
	i2c_start(DS3231_ADDRESS+I2C_READ);
	
	tmp = i2c_readAck();					// secounds
	tmp &= 0x7F;
	ds3231.secounds = tmp;
	time_string[7] = ((tmp & 0x0F) + 48);
	time_string[6] = ((tmp >> 4) + 48);
	time_string[5] = ':';
	
	tmp = i2c_readAck();					// minutes
	tmp &= 0x7F;
	ds3231.minutes = tmp;
	time_string[4] = ((tmp & 0x0F) + 48);
	time_string[3] = ((tmp >> 4) + 48);
	time_string[2] = ':';
	
	tmp = i2c_readAck();					// hours
	tmp &= 0x7F;
	ds3231.hours = tmp;
	time_string[1] = ((tmp & 0x0F) + 48);
	time_string[0] = ((tmp >> 4) + 48);
	
	time_string[8] = '\0';					// end of string
	
#ifdef UART_OUTPUT
uart_puts(time_string);
uart_puts("\r\n");
#endif	

	
	ds3231.day_name = i2c_readAck();

	tmp = i2c_readAck();					// day
	ds3231.day = tmp;
	date_string[0] = ((tmp >> 4) + 48);
	date_string[1] = ((tmp & 0x0F) + 48);
	date_string[2] = '.';
	
	tmp = i2c_readAck();					// month
	ds3231.month = tmp;
	date_string[3] = ((tmp >> 4) + 48);
	date_string[4] = ((tmp & 0x0F) + 48);
	date_string[5] = '.';

	tmp = i2c_readNak();					// year
	ds3231.year = tmp;
	date_string[6]  = '2';
	date_string[7]  = '0';
	date_string[8] = ((tmp >> 4) + 48);
	date_string[9] = ((tmp & 0x0F) + 48);
	date_string[10] = ' ';
	
	switch (ds3231.day_name)
	{
		case 1:
		{
			date_string[11] = 'S';
			date_string[12] = 'o';
		}break;
		case 2:
		{
			date_string[11] = 'M';
			date_string[12] = 'o';
		}break;
		case 3:
		{
			date_string[11] = 'D';
			date_string[12] = 'i';
		}break;
		case 4:
		{
			date_string[11] = 'M';
			date_string[12] = 'i';
		}break;
		case 5:
		{
			date_string[11] = 'D';
			date_string[12] = 'o';
		}break;
		case 6:
		{
			date_string[11] = 'F';
			date_string[12] = 'r';
		}break;
		case 7:
		{
			date_string[11] = 'S';
			date_string[12] = 'a';
		}break;

	}
	
	date_string[13] = '\0';
	
#ifdef UART_OUTPUT
uart_puts(date_string);
uart_puts("\r\n");
#endif

	i2c_stop();
}

void DS3231_set_Time(uint8_t hours, uint8_t minutes, uint8_t secounds)
{
	
	if (hours < 0 || hours > 24 || minutes < 0 || minutes > 59 || secounds < 0 || secounds > 59)
	{
		return;
	}
	
	uint8_t set_time_string[3];
	uint8_t tmp;
	
	i2c_start(DS3231_ADDRESS+I2C_WRITE);
	i2c_write(SECOND);
	set_time_string[0] = (secounds / 10 ) << 4;   // ten
	set_time_string[1] = (secounds % 10 );      // one
	tmp = set_time_string[0] | set_time_string[1];
	i2c_write(tmp);
	set_time_string[0] = (minutes / 10 ) << 4;   // ten
	set_time_string[1] = (minutes % 10 );      // one
	tmp = set_time_string[0] | set_time_string[1];
	i2c_write(tmp);
	set_time_string[0] = (hours / 10 ) << 4;   // ten
	set_time_string[1] = (hours % 10 );      // one
	tmp = set_time_string[0] | set_time_string[1];
	i2c_write(tmp);
	i2c_stop();
}

void DS3231_set_Date(uint8_t day_name, uint8_t day, uint8_t month, uint8_t year)
{
	if (day_name < 0 || day_name > 8 || day < 1 || day > 31 || month < 1 || month > 12 || year < 15 || year > 99)
	{
	}
	
	uint8_t set_date_string[3];
	uint8_t tmp;
	
	i2c_start(DS3231_ADDRESS+I2C_WRITE);
	i2c_write(DAY);
	i2c_write(day_name);
	set_date_string[0] = (day / 10 ) << 4;   // ten
	set_date_string[1] = (day % 10 );      // one
	tmp = set_date_string[0] | set_date_string[1];
	i2c_write(tmp);
	set_date_string[0] = (month / 10 ) << 4;   // ten
	set_date_string[1] = (month % 10 );      // one
	tmp = set_date_string[0] | set_date_string[1];
	i2c_write(tmp);
	set_date_string[0] = (year / 10 ) << 4;   // ten
	set_date_string[1] = (year % 10 );      // one
	tmp = set_date_string[0] | set_date_string[1];
	i2c_write(tmp);
	i2c_stop();
}
