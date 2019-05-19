
/*************************************************************************************
*				Generate a Time String (12:00:00) or Date String (12.01.2015 Mo)
*	Autor	: Jan Homann
*	Version : 1.0
*	Date	: 26.11.2015
***************************************************************************************/


#include <avr/io.h>
#include <stdlib.h>
#include "ttostr.h"



/* contains the time string */
typedef union
{
	char Time[11];
	char Date[16];	
}TtStr_u;

static TtStr_u TtStr;


char *bcd_ttostr( uint8_t Hour , uint8_t Minute , uint8_t Secound , enum TTOSTR_OPTIONS Options )
{
	if ( Options >  TTOSTR_HH_MM_SS )
	{
		return NULL;
	}
	
	switch( Options )
	{
		case TTOSTR_HH:
		{
			TtStr.Time[0] = ( Hour >> 4 ) + 48;
			TtStr.Time[1] = ( Hour & 0x0F ) + 48;
			TtStr.Time[2] = '\0';
		}break;
		
		case TTOSTR_MM:
		{
			TtStr.Time[0] = ( Minute >> 4 ) + 48;
			TtStr.Time[1] = ( Minute & 0x0F ) + 48;
			TtStr.Time[2] = '\0';
		}break;
		
		case TTOSTR_SS:
		{
			TtStr.Time[0] = ( Secound >> 4 ) + 48;
			TtStr.Time[1] = ( Secound & 0x0F ) + 48;
			TtStr.Time[2] = '\0';
		}break;
		
		case TTOSTR_HH_MM:
		{
			TtStr.Time[0] = ( Hour >> 4) + 48;
			TtStr.Time[1] = ( Hour & 0x0F ) + 48;
			TtStr.Time[2] = ':';
			TtStr.Time[3] = ( Minute >> 4 ) + 48;
			TtStr.Time[4] = ( Minute & 0x0F ) + 48;
			TtStr.Time[5] = '\0';
		}break;
		
		case TTOSTR_HH_MM_SS:
		{
			TtStr.Time[0] = ( Hour >> 4 ) + 48;
			TtStr.Time[1] = ( Hour & 0x0F ) + 48;
			TtStr.Time[2] = ':';
			TtStr.Time[3] = ( Minute >> 4 ) + 48;
			TtStr.Time[4] = ( Minute & 0x0F ) + 48;
			TtStr.Time[5] = ':';
			TtStr.Time[6] = ( Secound >> 4 ) + 48;
			TtStr.Time[7] = ( Secound & 0x0F ) + 48;
			TtStr.Time[8] = '\0';
		}break;
		
	}
	
	return TtStr.Time;
}

char *dec_ttostr( uint8_t Hour , uint8_t Minute , uint8_t Secound )
{
	/* DEC to ASCII */
	if ( Hour == 0xff )
	{
		TtStr.Time[0] = ' ';
		TtStr.Time[1] = ' ';
	}else
	{
		TtStr.Time[0] = ( ( Hour / 10 ) ) + 48;
		TtStr.Time[1] = ( Hour % 10 ) + 48;		
	}
	
	TtStr.Time[2] = ':';	
	
	if ( Minute == 0xff )
	{
		TtStr.Time[3] = ' ';
		TtStr.Time[4] = ' ';		
	}else
	{
		TtStr.Time[3] = ( ( Minute / 10 ) ) + 48;
		TtStr.Time[4] = ( Minute % 10 ) + 48;		
	}
	
	TtStr.Time[5] = ':';
	
	if ( Secound == 0xff )
	{
		TtStr.Time[6] = ' ';
		TtStr.Time[7] = ' ';	
	}else
	{
		TtStr.Time[6] = ( ( Secound / 10 ) ) + 48;
		TtStr.Time[7] = ( Secound % 10 ) + 48;
	}
	
	TtStr.Time[8] = '\0';
	
	return TtStr.Time;
}

char *bcd_dtostr( uint8_t Day , uint8_t Month , uint16_t Year , char NameOfDay)
{
	uint8_t tmp;
	
	/* BCD  to ASCII */
	TtStr.Date[0]		= ( Day >> 4 ) + 48; // ten
	TtStr.Date[1]		= ( Day & 0x0F ) + 48; // ones
	TtStr.Date[2]		= '.'; // point
	TtStr.Date[3]		= ( Month >> 4 ) + 48; // ten
	TtStr.Date[4]		= ( Month & 0x0F ) + 48; // ones
	TtStr.Date[5]		= '.'; // point
	
	TtStr.Date[6]		= ( Year / 1000 ) + 48; // thousand 
	tmp			= Year % 1000;
	
	TtStr.Date[7]		= ( tmp / 100 ) + 48; // hounder
	tmp			= tmp % 100;
	
	Year -= 2000;
	
	TtStr.Date[8]		=  ( tmp >> 4 ) + 48; // tens
	TtStr.Date[9]		=  ( tmp & 0x0F ) + 48;
	TtStr.Date[10]	= ' '; // empty 

	switch( NameOfDay )
	{
		case 1 :{TtStr.Date[11] = 'S'; TtStr.Date[12] = 'o';}break; // So
		case 2 :{TtStr.Date[11] = 'M'; TtStr.Date[12] = 'o';}break; // Mo
		case 3 :{TtStr.Date[11] = 'D'; TtStr.Date[12] = 'i';}break; // Di
		case 4 :{TtStr.Date[11] = 'M'; TtStr.Date[12] = 'i';}break; // Mi
		case 5 :{TtStr.Date[11] = 'D'; TtStr.Date[12] = 'o';}break; // Do
		case 6 :{TtStr.Date[11] = 'F'; TtStr.Date[12] = 'r';}break; // Fr
		case 7 :{TtStr.Date[11] = 'S'; TtStr.Date[12] = 'a';}break;	// Sa
		
		default :{TtStr.Date[10] = '\0';}break; // string terMinuteation
	}

	TtStr.Date[13] = '\0'; // string terMinuteation
	
	return TtStr.Date;
}

char *dec_dtostr( uint8_t Day , uint8_t Month , uint16_t Year , char NameOfDay )
{	
	/* DEC  to ASCII */
	if ( Day > 31 )
	{
		TtStr.Date[0]		= ' '; // ten
		TtStr.Date[1]		= ' '; // ones		
	}else
	{
		TtStr.Date[0]		= ( Day / 10 ) + 48; // ten
		TtStr.Date[1]		= ( Day % 10 ) + 48; // ones		
	}

	TtStr.Date[2]			= '.'; // point
	
	if ( Month > 12 )
	{
		TtStr.Date[3]		= ' '; // ten
		TtStr.Date[4]		= ' '; // ones	
	}else
	{
		TtStr.Date[3]		= ( Month / 10 ) + 48; // ten
		TtStr.Date[4]		= ( Month % 10 ) + 48; // ones		
	}

	TtStr.Date[5]		= '.'; // point
	
	uint16_t tmp = Year;
	tmp -= 2000;
	
	TtStr.Date[6]		= '2'; // thousand
	TtStr.Date[7]		= '0'; // hounder	
	
	if ( Year > 99 )
	{
		TtStr.Date[8]		=  ' '; // tens
		TtStr.Date[9]		=  ' ';	
	}else
	{
		TtStr.Date[8]		=  ( tmp / 10 ) + 48; // tens
		TtStr.Date[9]		=  ( tmp % 10 ) + 48;			
	}
	
	TtStr.Date[10]	= ' '; // empty

	const char *NameOfDayTable[] =
	{
		" " , "So" , "Mo" , "Di" , "Mi" , "Do" , "Fr" , "Sa"
	};

	if ( NameOfDay >= 1 && NameOfDay <= 7 )
	{
		TtStr.Date[11] = *(NameOfDayTable+NameOfDay)[0];
		TtStr.Date[12] = *(NameOfDayTable+NameOfDay)[1];
	}
	else
	{
		TtStr.Date[11] = ' ';
		TtStr.Date[12] = ' ';
	}
	
	TtStr.Date[13] = '\0'; // Termination

	return TtStr.Date;
}

