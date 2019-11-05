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
	for ( uint8_t x = 0 ; sTab[x].uiPort != NULL ; x++ )
	{
		switch ( ( sTab[x].uiFuncBitPos & 0xF0 ) )
		{
			case _FUNCTION_INPUT:
			{
				*( sTab[x].uiPort - 1 ) &= ~( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) ); // - 1 für das DDR Register
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				*( sTab[x].uiPort - 1 ) &= ~( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) ); // - 1 für das DDR Register
				*( sTab[x].uiPort ) |=  ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
			}break;
			
			case _FUNCTION_OUTPUT:
			{
				*( sTab[x].uiPort - 1 ) |= ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) ); // - 1 für das DDR Register
				*( sTab[x].uiPort ) |= ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
			}break;
			
			
			case _FUNCTION_INPUT_PULLDOWN:{}
		}
	}
}

void IoTabSetLow( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].uiPort != NULL ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & 0xF0 ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].uiPort ) &= ~( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabSetHigh( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].uiPort != NULL  ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & 0xF0 ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].uiPort ) |= ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabxHigh( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].uiPort == NULL )
	{
		return; // Ungültig!
	}
	
	*(sTab[uiMember].uiPort) |= ( 1 << sTab[uiMember].uiFuncBitPos & 0x0F );
}

void IoTabxLow( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].uiPort == NULL )
	{
		return; // Ungültig!
	}
	
	*(sTab[uiMember].uiPort) &= ~( 1 << sTab[uiMember].uiFuncBitPos & 0x0F );
}

/*****************************************************************/
// end of file
