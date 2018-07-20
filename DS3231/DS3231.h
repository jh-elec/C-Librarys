/*************************************************************************************
*				Driver for " DS3231 RTC "
*	Autor	: Jan Homann
*	Version : 1.0
*	Date	: 11.12.2015
***************************************************************************************/

#define SECOND             0x00
#define MINUTE             0x01
#define HOUR               0x02
#define DAY                0x03
#define DATE               0x04
#define MONTH              0x05
#define YEAR               0x06
#define ALARM_1_SECONDS    0x07
#define ALARM_1_MINUTES    0x08
#define ALARM_1_HOURS      0x09
#define ALARM_1_DAY_DATE   0x0A
#define ALARM_2_MINUTES    0x0B
#define ALARM_2_HOURS      0x0C
#define ALARM_2_DAY_DATE   0x0D
#define CONTROL            0x0E
#define STATUS             0x0F
#define AGING_OFFSET       0x10
#define MSB_TEMP           0x11
#define LSB_TEMP           0x12

#define AT24C32_ADDRESS    0xAC
#define DS3231_ADDRESS 0xD0


struct rtc_data
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t secounds;

	uint8_t day_name;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	
};typedef struct rtc_data rtc;

extern rtc ds3231;

/***************************************************************************************
*	set the output frequency ( all parameters are dec! ) 
*
*	Parameter :
*				1 = 1Hz
*				2 = 1.024kHz
*				3 = 4.096kHz
*				4 = 8.192kHz
***************************************************************************************/
void DS3231_set_SWOF(uint8_t clock);

/***************************************************************************************
* read date and time ( all parameters are dec! )
***************************************************************************************/
void DS3231_get_Data(void);

/***************************************************************************************
*	set time ( all parameters are dec! )
*
*	Parameter :
*				hours	 = z.B 12
*				minutes  = z.B 05
*				secounds = z.B 00
***************************************************************************************/
void DS3231_set_Time(uint8_t hours, uint8_t minutes, uint8_t secounds);

/***************************************************************************************
*	set date ( all parameters are dec! ) 
*
*	Parameter :
*				day_name = 1 = Sunday , 2 = Monday , 3 = Tuesday , 4 = Wednesday , 5 = Thursday , 6 = Friday , 7 = Saturday 
*				day		 = z.B 05 
*				month	 = z.B 07 
*				year     = z.B 15 
***************************************************************************************/
void DS3231_set_Date(uint8_t day_name, uint8_t day, uint8_t month, uint8_t year);


