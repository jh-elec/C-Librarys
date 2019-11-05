





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
	
	
	__END_OF_MAPPING__
};
/*****************************************************************/

/*!<-- Mapping Table -- >*/
/*****************************************************************/
const sIO_t sMapping[__END_OF_MAPPING__]=
{
	[IO_GLCD_CS1]         = { &PORTA , IO_TAB_PINFUNC( 7 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_RES]         = { &PORTA , IO_TAB_PINFUNC( 6 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_A0]          = { &PORTA , IO_TAB_PINFUNC( 5 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_SCL]         = { &PORTA , IO_TAB_PINFUNC( 4 , _FUNCTION_OUTPUT ) },
	[IO_GLCD_DATA]        = { &PORTA , IO_TAB_PINFUNC( 3 , _FUNCTION_OUTPUT ) },
	[IO_LED_BLUE]         = { &PORTC , IO_TAB_PINFUNC( 7 , _FUNCTION_OUTPUT ) },
	[IO_LED_GREEN]        = { &PORTC , IO_TAB_PINFUNC( 6 , _FUNCTION_OUTPUT ) },
	[IO_LED_RED]          = { &PORTC , IO_TAB_PINFUNC( 5 , _FUNCTION_OUTPUT ) },
	[IO_LED_HEARTBEAT]    = { &PORTB , IO_TAB_PINFUNC( 7 , _FUNCTION_OUTPUT ) },
	[IO_SOUND]            = { &PORTC , IO_TAB_PINFUNC( 4 , _FUNCTION_OUTPUT ) },
	[IO_VIB_MOTOR]        = { &PORTB , IO_TAB_PINFUNC( 6 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_1]         = { &PORTD , IO_TAB_PINFUNC( 2 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_2]         = { &PORTD , IO_TAB_PINFUNC( 3 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_3]         = { &PORTD , IO_TAB_PINFUNC( 4 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_4]         = { &PORTD , IO_TAB_PINFUNC( 5 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_5]         = { &PORTD , IO_TAB_PINFUNC( 6 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_6]         = { &PORTD , IO_TAB_PINFUNC( 7 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_7]         = { &PORTC , IO_TAB_PINFUNC( 2 , _FUNCTION_OUTPUT ) },
	[IO_RELAIS_8]         = { &PORTC , IO_TAB_PINFUNC( 3 , _FUNCTION_OUTPUT ) },


	[IO_ENC_A]            = { &PORTA , IO_TAB_PINFUNC( 2 , _FUNCTION_INPUT_PULLUP ) },
	[IO_ENC_B]            = { &PORTA , IO_TAB_PINFUNC( 1 , _FUNCTION_INPUT_PULLUP ) },
	[IO_MEASURE_VCC]      = { &PORTA , IO_TAB_PINFUNC( 0 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_ENTER]     = { &PORTB , IO_TAB_PINFUNC( 3 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_UP]        = { &PORTB , IO_TAB_PINFUNC( 2 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_DOWN]      = { &PORTB , IO_TAB_PINFUNC( 1 , _FUNCTION_INPUT_PULLUP ) },
	[IO_SWITCH_RETURN]    = { &PORTB , IO_TAB_PINFUNC( 0 , _FUNCTION_INPUT_PULLUP ) },

	{ NULL , 0 },

	// 	[IO_I2C_SCL]          = { &PORTC , 0 | _FUNCTION_OUTPUT },
	// 	[IO_I2C_SDA]          = { &PORTC , 1 | _FUNCTION_INPUT_PULLUP },
	//
	// 	[IO_UART_TX]          = { &PORTD , 1 | _FUNCTION_INPUT_PULLUP },
	// 	[IO_UART_RX]          = { &PORTD , 0 | _FUNCTION_INPUT_PULLUP },
};
/*****************************************************************/