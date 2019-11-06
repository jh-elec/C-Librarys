/*************************************************************
*|
*|	\@file  	iotab.h
*|	\@brief
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Strukturierte Handhabung der Ein. bzw. Ausgänge
*|
*|	\@date		30/01/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.301019
*|
*|**************************************************************/

#ifndef __IOTAB_H__
 #include "../includes/iotab.h"
#endif





/*!<-- functions -- >*/
/*****************************************************************/

void IoTabInit( const sIO_t *sTab )
{	
	uint8_t uiPinAction = 0;
	uint8_t uiPinPos = 0;
	
	for ( uint8_t uiTabPos = 0 ; sTab[uiTabPos].pPort != NULL ; uiTabPos++ )
	{
		uiPinAction = sTab[uiTabPos].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK;
		uiPinPos    = sTab[uiTabPos].uiFuncBitPos & IO_TAB_PIN_MASK;
		
		if ( uiPinAction & _FUNCTION_OUTPUT )
		{
			*( sTab[uiTabPos].pPort - 1 ) |= ( 1 << uiPinPos ); // - 1 für das DDR Register
					
			if ( uiPinAction & IO_TAB_DEFAULT_MASK ) // Default Zustand
			{
				*( sTab[uiTabPos].pPort ) |= ( 1 << uiPinPos );
			}else
			{
				*( sTab[uiTabPos].pPort ) &= ~( 1 << ( uiPinPos ) );
			}
		}
		else if ( uiPinAction & _FUNCTION_INPUT )
		{
			*( sTab[uiTabPos].pPort - 1 ) &= ~( 1 << ( uiPinPos ) ); // - 1 für das DDR Register
		}
		else if ( uiPinAction & _FUNCTION_INPUT_PULLUP )
		{				
 			*( sTab[uiTabPos].pPort - 1 ) &= ~( 1 << ( uiPinPos ) ); // - 1 für das DDR Register
 			*( sTab[uiTabPos].pPort     ) |=  ( 1 << ( uiPinPos ) );
		}
	}
}

void IoTabSetLow( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].pPort != NULL ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].pPort ) &= ~( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabSetHigh( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].pPort != NULL  ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].pPort ) |= ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabxHigh( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].pPort == NULL )
	{
		return; // Ungültig!
	}
	
	*(sTab[uiMember].pPort) |= ( 1 << ( sTab[uiMember].uiFuncBitPos & 0x0F ) );
}

void IoTabxLow( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].pPort == NULL )
	{
		return; // Ungültig!
	}
	
	*(sTab[uiMember].pPort) &= ~( 1 << ( sTab[uiMember].uiFuncBitPos & 0x0F ) );
}

void IoTabxToggle( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].pPort == NULL )
	{
		return; // Ungültig!
	}
	
	*(sTab[uiMember].pPort) ^= 1 << ( sTab[uiMember].uiFuncBitPos & 0x0F ) ;
}

/*****************************************************************/
// end of file
