/*************************************************************
*|
*|	\@file  	map.h
*|	\@brief
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Adressen und Funktionen des Port(-Pins)
*|
*|	\@date		06/01/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.061119
*|
*|**************************************************************/

#include "iotab.h"




/*!<-- Enum for Mapping -- >*/
/*****************************************************************/
enum __PACK_IT__ eMapping
{	
	LCD_LIGHT,
	LCD_CONTRAST,
	
	STEPPER_DIR,
	STEPPER_PULSE,
	STEPPER_EN,
	
	LED_1,
	LED_2,
	LED_3,
	LED_4,
	
	IO_END,

	__END_OF_MAPPING__,    
};
/*****************************************************************/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Bsp. = [LCD_READ_WRITE]	= { &PORTD , IO_TAB_CNFG( PD7 , 0 , _FUNCTION_OUTPUT ) },
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	
	
/*!<-- Mapping Table -- >*/	
/*****************************************************************/
const sIO_t sMapping[__END_OF_MAPPING__]=
{
	[LCD_CONTRAST]		= { &PORTD , IO_TAB_CNFG( PD5 , 0 , _FUNCTION_OUTPUT ) },
	[LCD_LIGHT]			= { &PORTD , IO_TAB_CNFG( PD4 , 0 , _FUNCTION_OUTPUT ) },
		
// 	[ENCODER_A]			= { &PORTC , IO_TAB_CNFG( PC5 , 0 , _FUNCTION_INPUT ) },
// 	[ENCODER_B]			= { &PORTC , IO_TAB_CNFG( PC4 , 0 , _FUNCTION_INPUT ) },
// 	[ENCODER_ENTER]		= { &PORTC , IO_TAB_CNFG( PC3 , 0 , _FUNCTION_INPUT ) },
	
	[STEPPER_PULSE]		= { &PORTB , IO_TAB_CNFG( PB4 , 0 , _FUNCTION_OUTPUT ) },
	[STEPPER_DIR]		= { &PORTB , IO_TAB_CNFG( PB3 , 0 , _FUNCTION_OUTPUT ) },
	[STEPPER_EN]		= { &PORTB , IO_TAB_CNFG( PB2 , 0 , _FUNCTION_OUTPUT ) },
	
	[LED_1]				= { &PORTA , IO_TAB_CNFG( PA3 , 1 , _FUNCTION_OUTPUT ) },
	[LED_2]				= { &PORTA , IO_TAB_CNFG( PA2 , 1 , _FUNCTION_OUTPUT ) },
	[LED_3]				= { &PORTA , IO_TAB_CNFG( PA1 , 1 , _FUNCTION_OUTPUT ) },
	[LED_4]				= { &PORTA , IO_TAB_CNFG( PA0 , 1 , _FUNCTION_OUTPUT ) },
	
	[IO_END]              = { NULL , 0 },
};

//uint8_t uiSizeOfMapping = sizeof( sMapping ) / sizeof( *sMapping );

/*****************************************************************/