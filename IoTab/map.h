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
	IO_GLCD_CS1,
	IO_GLCD_RES,
	IO_GLCD_A0,
	IO_GLCD_SCL,
	IO_GLCD_DATA,	
	IO_LED_BLUE,
	IO_LED_GREEN,
	IO_LED_RED,
	IO_LED_HEARTBEAT,
	IO_ENC_A,
	IO_ENC_B,
	IO_MEASURE_VCC,
	IO_VIB_MOTOR,
	IO_SWITCH_ENTER,
	IO_SWITCH_UP,
	IO_SWITCH_DOWN,
	IO_SWITCH_RETURN,
	IO_SOUND,
	IO_RELAIS_1,
	IO_RELAIS_2,
	IO_RELAIS_3,
	IO_RELAIS_4,
	IO_RELAIS_5,
	IO_RELAIS_6,
	IO_RELAIS_7,
	IO_RELAIS_8,
	IO_I2C_SCL,
	IO_I2C_SDA,
	IO_UART_TX,
	IO_UART_RX,

	IO_END,

	__END_OF_MAPPING__,    
};
/*****************************************************************/

/*!<-- Mapping Table -- >*/
/*****************************************************************/
const sIO_t sMapping[__END_OF_MAPPING__]=
{
	[IO_GLCD_CS1]         = { &PORTA , IO_TAB_CNFG( PA7 , 0 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_RES]         = { &PORTA , IO_TAB_CNFG( PA6 , 0 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_A0]          = { &PORTA , IO_TAB_CNFG( PA5 , 0 , _FUNCTION_OUTPUT ) },		
	[IO_GLCD_SCL]         = { &PORTA , IO_TAB_CNFG( PA4 , 0 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_DATA]        = { &PORTA , IO_TAB_CNFG( PA3 , 0 , _FUNCTION_OUTPUT ) },
			
	[IO_LED_BLUE]         = { &PORTC , IO_TAB_CNFG( PC7 , 0 , _FUNCTION_OUTPUT ) },
	[IO_LED_GREEN]        = { &PORTC , IO_TAB_CNFG( PC6 , 0 , _FUNCTION_OUTPUT ) },
	[IO_LED_RED]          = { &PORTC , IO_TAB_CNFG( PC5 , 0 , _FUNCTION_OUTPUT ) },
	[IO_LED_HEARTBEAT]    = { &PORTB , IO_TAB_CNFG( PB7 , 0 , _FUNCTION_OUTPUT ) },
		
	[IO_VIB_MOTOR]        = { &PORTB , IO_TAB_CNFG( PB6 , 0 , _FUNCTION_OUTPUT ) },			
	[IO_SOUND]            = { &PORTC , IO_TAB_CNFG( PC4 , 0 , _FUNCTION_OUTPUT ) },		
		
	[IO_RELAIS_1]         = { &PORTD , IO_TAB_CNFG( PD2 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_2]         = { &PORTD , IO_TAB_CNFG( PD3 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_3]         = { &PORTD , IO_TAB_CNFG( PD4 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_4]         = { &PORTD , IO_TAB_CNFG( PD5 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_5]         = { &PORTD , IO_TAB_CNFG( PD6 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_6]         = { &PORTD , IO_TAB_CNFG( PD7 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_7]         = { &PORTC , IO_TAB_CNFG( PC2 , HIGH , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_8]         = { &PORTC , IO_TAB_CNFG( PC3 , HIGH , _FUNCTION_OUTPUT ) },		
		
	[IO_ENC_A]            = { &PORTA , IO_TAB_CNFG( PA2 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_ENC_B]            = { &PORTA , IO_TAB_CNFG( PA1 , 0 , _FUNCTION_INPUT_PULLUP ) },		
	
	[IO_MEASURE_VCC]      = { &PORTA , IO_TAB_CNFG( PA0 , 0 , _FUNCTION_INPUT_PULLUP ) },		
	
	[IO_SWITCH_ENTER]     = { &PORTB , IO_TAB_CNFG( PB3 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_UP]        = { &PORTB , IO_TAB_CNFG( PB2 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_DOWN]      = { &PORTB , IO_TAB_CNFG( PB1 , 0 , _FUNCTION_INPUT_PULLUP ) },	
	[IO_SWITCH_RETURN]    = { &PORTB , IO_TAB_CNFG( PB0 , 0 , _FUNCTION_INPUT_PULLUP ) },		

	[IO_I2C_SCL]          = { &PORTC , IO_TAB_CNFG( PC0 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_I2C_SDA]          = { &PORTC , IO_TAB_CNFG( PC1 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_UART_TX]          = { &PORTD , IO_TAB_CNFG( PD1 , 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_UART_RX]          = { &PORTD , IO_TAB_CNFG( PD0 , 0 , _FUNCTION_INPUT_PULLUP ) },		
	
	[IO_END]              = { NULL , 0 },
};

//uint8_t uiSizeOfMapping = sizeof( sMapping ) / sizeof( *sMapping );

/*****************************************************************/