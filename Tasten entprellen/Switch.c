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

/* Prototypen */

enum eSwitchError SwitchInit( sSwitch_t *psSwitch , pRegister_t pPort , uint8_t uiSwitchMask )
{
	if ( pPort != NULL )
	{
		PORT_DDR_ADDR( pPort ) &= ~( uiSwitchMask );
	}
	else
	{
		return ERROR_SWITCH_NO_PORT;
	}

	psSwitch->pPort = pPort;
	psSwitch->Mask = uiSwitchMask;
	psSwitch->Old = 0;
	psSwitch->New = 0;
	psSwitch->Info = 0;
	
	return ERROR_SWITCH_OK;
}

static uint8_t SwitchRead( sSwitch_t *psSwitch )
{
	psSwitch->New = ( ( PORT_PIN_ADDR( psSwitch->pPort ) & psSwitch->Mask ) ^ psSwitch->Mask );
	
	if ( psSwitch->New != psSwitch->Old ) // Eingang geaendert
	{
		psSwitch->Info = ( psSwitch->Info | ( psSwitch->New & ( psSwitch->Old ^ psSwitch->New ) ) );
	}
	
	psSwitch->Old = psSwitch->New;
	
	return psSwitch->Info;
}

uint8_t SwitchGet( sSwitch_t *psSwitch , uint8_t uiMask )
{
	uint8_t uiInfo;
	
	SwitchRead( psSwitch );
	
	uiInfo = psSwitch->Info;
	psSwitch->Info &= ~uiMask;
	
	return ( uiInfo & uiMask );
}

void SwitchClear( sSwitch_t *psSwitch )
{
	psSwitch->Old = 0;
	psSwitch->New = 0;
	psSwitch->Info = 0;
}

/* Ende Prototypen */