
/*************************************************************************************
*				Generate a Time String (12:00:00) or Date String (12.01.2015 Mo)
*	Autor	: Jan Homann
*	Version : 1.0
*	Date	: 26.11.2015
***************************************************************************************/


#include <avr/io.h>
#include "ttostr.h"


/* contains the time string */
char time[11];
char date[16];


char *bcd_ttostr(uint8_t hour, uint8_t min, uint8_t sec)
{
	/* BCD to ASCII */
	time[0] = (hour >> 4) + 48;
	time[1] = (hour & 0x0F) + 48;
	time[2] = ':';
	time[3] = (min >> 4) + 48;
	time[4] = (min & 0x0F) + 48;
	time[5] = ':';
	time[6] = (sec >> 4) + 48;
	time[7] = (sec & 0x0F) + 48;
	time[8] = '\n';
	time[9] = '\r';
	time[10] = '\0';
	
	return time;
}

char *dec_ttostr(uint8_t hour, uint8_t min, uint8_t sec)
{
	/* DEC to ASCII */
	if (hour==0xff){
		time[0] = 'x';
		time[1] = 'x';
	}else{
		time[0] = ((hour / 10)) + 48;
		time[1] = (hour % 10) + 48;		
	}
	
	time[2] = ':';	
	
	if (min==0xff){
		time[3] = 'x';
		time[4] = 'x';		
	}else{
		time[3] = ((min / 10)) + 48;
		time[4] = (min % 10) + 48;		
	}
	
	time[5] = ':';
	
	if (sec==0xff){
		time[6] = 'x';
		time[7] = 'x';	
	}else{
		time[6] = ((sec / 10)) + 48;
		time[7] = (sec % 10) + 48;
	}
	
	time[8] = '\n';
	time[9] = '\r';
	time[10] = '\0';
	
	return time;
}

char *bcd_dtostr(uint8_t day, uint8_t month, uint16_t year, char Day_Name)
{
	uint8_t tmp;
	
	/* BCD  to ASCII */
	date[0]		= (day >> 4) + 48; // ten
	date[1]		= (day & 0x0F) + 48; // ones
	date[2]		= '.'; // point
	date[3]		= (month >> 4) + 48; // ten
	date[4]		= (month & 0x0F) + 48; // ones
	date[5]		= '.'; // point
	
	date[6]		= (year / 1000) + 48; // thousand 
	tmp			= year % 1000;
	
	date[7]		= (tmp / 100) + 48; // hounder
	tmp			= tmp % 100;
	
	year -= 2000;
	
	date[8]		=  (tmp >> 4) + 48; // tens
	date[9]		=  (tmp & 0x0F) + 48;
	date[10]	= ' '; // empty 

	switch(Day_Name)
	{
		case 1 :{date[11] = 'S'; date[12] = 'o';}break; // So
		case 2 :{date[11] = 'M'; date[12] = 'o';}break; // Mo
		case 3 :{date[11] = 'D'; date[12] = 'i';}break; // Di
		case 4 :{date[11] = 'M'; date[12] = 'i';}break; // Mi
		case 5 :{date[11] = 'D'; date[12] = 'o';}break; // Do
		case 6 :{date[11] = 'F'; date[12] = 'r';}break; // Fr
		case 7 :{date[11] = 'S'; date[12] = 'a';}break;	// Sa
		
		default :{date[10] = '\0';}break; // string termination
	}
	
	date[13] = '\n';
	date[14] = '\r';
	date[15] = '\0'; // string termination

	
	
	return date;
}

char *dec_dtostr(uint8_t day, uint8_t month, uint16_t year, char Day_Name)
{	
	/* DEC  to ASCII */
	if (day==0xff){
		date[0]		= 'x'; // ten
		date[1]		= 'x'; // ones		
	}else{
		date[0]		= (day / 10) + 48; // ten
		date[1]		= (day % 10) + 48; // ones		
	}

	date[2]		= '.'; // point
	
	if (month==0xff){
		date[3]		= 'x'; // ten
		date[4]		= 'x'; // ones	
	}else{
		date[3]		= (month / 10) + 48; // ten
		date[4]		= (month % 10) + 48; // ones		
	}

	date[5]		= '.'; // point
	
	uint16_t tmp = year;
	tmp-=2000;
	
	date[6]		= '2'; // thousand
	date[7]		= '0'; // hounder	
	
	if (year==0xffff){
		date[8]		=  'x'; // tens
		date[9]		=  'x';	
	}else{
		date[8]		=  (tmp / 10) + 48; // tens
		date[9]		=  (tmp % 10) + 48;			
	}
	
	date[10]	= ' '; // empty

	switch(Day_Name)
	{
		case 0 :{date[10] = '\0'			   ;}break; // string termination
		case 1 :{date[11] = 'S'; date[12] = 'o';}break; // So
		case 2 :{date[11] = 'M'; date[12] = 'o';}break; // Mo
		case 3 :{date[11] = 'D'; date[12] = 'i';}break; // Di
		case 4 :{date[11] = 'M'; date[12] = 'i';}break; // Mi
		case 5 :{date[11] = 'D'; date[12] = 'o';}break; // Do
		case 6 :{date[11] = 'F'; date[12] = 'r';}break; // Fr
		case 7 :{date[11] = 'S'; date[12] = 'a';}break;	// Sa
		
		default :{date[11] = 'x'; date[12] = 'x';}break; // empty space = x
	}
	
	date[13] = '\n';
	date[14] = '\r';
	date[15] = '\0'; // string termination

	
	
	return date;
}

