/*************************************************************************************
*				Generate a Time String (12:00:00) or Date String (12.01.2015 Mo)
*	Autor	: Jan Homann
*	Version : 1.0
*	Date	: 26.11.2015
***************************************************************************************/




#ifndef F_CPU
#define F_CPU 8000000
#endif

enum TTOSTR_OPTIONS
{
	TTOSTR_HH,
	TTOSTR_MM,
	TTOSTR_SS,
	TTOSTR_HH_MM,
	TTOSTR_HH_MM_SS,
};

enum Days	{
				So = 1,
				Mo,
				Di,
				Mi,
				Do,
				Fr,
				Sa
			};


#define	NO_DAY_NAME	0



/***PROTOTYPES***/



/*****************************************************************
* description : make a string from uint8_t values (BCD Format!)
* parameter : hours, minutes, seconds , Length of the string
* returns : address of time string
* sample : 	lcd_puts(ttostr(12,05,01));
*****************************************************************/
char *bcd_ttostr( uint8_t Hour , uint8_t Minute , uint8_t Secound , enum TTOSTR_OPTIONS Options );

/*****************************************************************
* description : make a string from uint8_t values (DEC Format!)
* parameter : hours, minutes, seconds
* returns : address of time string
* sample : 	lcd_puts(ttostr(12,05,01));
*****************************************************************/
char *dec_ttostr(uint8_t hour, uint8_t min, uint8_t sec);

/*****************************************************************
* description : make a string from uint8_t / uint16_t values (BCD Format!)
* parameter : day, month, year
* returns : address of date string
* sample : 	lcd_puts(dtostr(13,01,2015,NO_DAY_NAME)); // no name for day
* sample : 	lcd_puts(dtostr(13,01,2015,Mo)); // name for day "Mo"
*****************************************************************/
char *bcd_dtostr(uint8_t day, uint8_t month, uint16_t year, char Day_Name);

/*****************************************************************
* description : make a string from uint8_t / uint16_t values (DEC Format!)
* parameter : day, month, year
* returns : address of date string
* sample : 	lcd_puts(dtostr(13,01,2015,NO_DAY_NAME)); // no name for day
* sample : 	lcd_puts(dtostr(13,01,2015,Mo)); // name for day "Mo"
*****************************************************************/
char *dec_dtostr(uint8_t day, uint8_t month, uint16_t year, char Day_Name);

