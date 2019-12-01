/*************************************************************
*|
*|	\@file  	Switch.c
*|	\@brief
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Taster einlesen und verarbeiten ( entprellen )
*|
*|	\@date		28/03/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.280319
*|
*|**************************************************************/

#include <avr/io.h>
#include "../includes/Switch.h"


volatile static uint8_t uiRepeat = 25;

/* Prototypen */

enum eSwitchError SwitchInit( sSwitch_t *psSwitch , pRegister_t pPort , uint8_t uiKeyMask , uint8_t uiPullUpMask , uint8_t uiRepeatKeyMask )
{
	if ( pPort != NULL )
	{
		PORT_DDR_ADDR( pPort ) &= ~( uiKeyMask );
		*pPort |= uiPullUpMask;
	}
	else
	{
		return ERROR_SWITCH_NO_PORT;
	}

	psSwitch->pPort = pPort;
	psSwitch->uiMask = uiKeyMask;
	psSwitch->uiOld = 0;
	psSwitch->uiNew = 0;
	psSwitch->uiInfo = 0;
	psSwitch->uiRepeat = 0;
	psSwitch->uiRepeatKeyMask = uiRepeatKeyMask;
	
	return ERROR_SWITCH_OK;
}

volatile uint8_t uiInfoRpt;

uint8_t SwitchRead( sSwitch_t *psSwitch )
{
	psSwitch->uiNew = ( ( PORT_PIN_ADDR( psSwitch->pPort ) & psSwitch->uiMask ) ^ psSwitch->uiMask );
	
	if ( psSwitch->uiNew != psSwitch->uiOld ) // Eingang geaendert
	{
		psSwitch->uiInfo = ( psSwitch->uiInfo | ( psSwitch->uiNew & ( psSwitch->uiOld ^ psSwitch->uiNew ) ) );
		uiRepeat = _SWITCH_REPEAT_START;
	}
		
	if ( --uiRepeat == 0 && psSwitch->uiNew & psSwitch->uiRepeatKeyMask )
	{
		uiRepeat = _SWITCH_REPEAT_NEXT;
		psSwitch->uiRepeat |= psSwitch->uiNew & psSwitch->uiRepeatKeyMask;
	}
	
	psSwitch->uiOld = psSwitch->uiNew;
	
	
	return psSwitch->uiInfo;
}

uint8_t SwitchGetRpt( sSwitch_t *psSwitch , uint8_t uiKeyMask )
{
	cli();
	uiKeyMask &= psSwitch->uiRepeat;
	psSwitch->uiRepeat ^= uiKeyMask;
	sei();

	return uiKeyMask;
}

uint8_t SwitchGet( sSwitch_t *psSwitch , uint8_t uiKeyMask )
{
	uint8_t uiInfo;
	
	cli();
	uiInfo = psSwitch->uiInfo;
	psSwitch->uiInfo &= ~uiKeyMask;
	sei();
	
	return ( uiInfo & uiKeyMask );
}


void SwitchClear( sSwitch_t *psSwitch )
{
	psSwitch->uiOld = 0;
	psSwitch->uiNew = 0;
	psSwitch->uiInfo = 0;
	psSwitch->uiRepeat = 0;
}




/* Ende Prototypen */