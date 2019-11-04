#include "iotab.h"






sIO_t IoTable[]=
{
	[IOTAB_POS_LED1]   = { &PORTA , 2 , _FUNCTION_OUTPUT },
	[IOTAB_POS_K1]     = { &PORTB , 3 , _FUNCTION_OUTPUT },
	[IOTAB_POS_BEEPER] = { &PORTC , 4 , _FUNCTION_OUTPUT },
};