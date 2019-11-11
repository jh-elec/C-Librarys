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

void SwitchInit( Switch_t *Switch )
{
	if ( Switch->pPort != NULL )
	{
		PORT_DDR_ADDR( Switch->pPort ) &= ~( Switch->Mask );
	}

	Switch->Old = 0;
	Switch->New = 0;
	Switch->Info = 0;
}

uint8_t SwitchRead( Switch_t *Switch )
{
	Switch->New = ( ( PORT_PIN_ADDR( Switch->pPort ) & Switch->Mask ) ^ Switch->Mask );
	
	if (Switch->New != Switch->Old) // Eingang geaendert
	{
		Switch->Info = ( Switch->Info | ( Switch->New & ( Switch->Old ^ Switch->New ) ) );
	}
	
	Switch->Old = Switch->New;
	
	return Switch->Info;
}

void SwitchClear( Switch_t *Switch )
{
	Switch->Old = 0;
	Switch->New = 0;
	Switch->Info = 0;
}

/* Ende Prototypen */