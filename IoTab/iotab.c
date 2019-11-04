/*************************************************************
*|
*|	\@file  	iotab.c
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

#include "iotab.c"



/*!<-- functions -- >*/
/*****************************************************************/

void            IoTabInit( sIO_t *sTab , uint8_t uiMembers )
{
	for ( uint8_t x = 0 ; x < uiMembers ; x++ )
	{
		switch ( sTab[x].uiPinFunc )
		{
			case _FUNCTION_INPUT:
			{
				*( sTab[x].uiPort - 1 ) &= ~( 1 << sTab[x].uiBitPos ); // - 1 für das DDR Register
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				*( sTab[x].uiPort - 1 ) &= ~( 1 << sTab[x].uiBitPos ); // - 1 für das DDR Register
				*( sTab[x].uiPort ) |=  ( 1 << sTab[x].uiBitPos );
			}break;
			
			case _FUNCTION_OUTPUT:
			{
				*( sTab[x].uiPort - 1 ) |= ( 1 << sTab[x].uiBitPos ); // - 1 für das DDR Register
				*( sTab[x].uiPort ) |= ( 1 << sTab[x].uiBitPos );
			}break;
			
			
			case _FUNCTION_INPUT_PULLDOWN:{}
			case __MAX_FUNCTION_ENTRYS__:{}
		}
	}
}

pRegister_t     IoTabGetPort( enum ePorts ePort )
{
	if ( ePort > __PORTS_MAX_ENTRYS__ )
	{
		return NULL; // Ungültig!
	}
	
	static const pRegister_t pPortLookup[ __PORTS_MAX_ENTRYS__ ] =
	{
		[ePORTA] = &PORTA,
		[ePORTB] = &PORTB,
		[ePORTC] = &PORTC,
		[ePORTD] = &PORTD,
	};

	return pPortLookup[ ePort ];
}

pRegister_t     IoTabGetDDRx( enum eDDRs eDDR )
{
	if ( eDDR > __DDRx_MAX_ENTRYS__ )
	{
		return NULL; // Ungültig!
	}
	
	static const pRegister_t pDDRxLookup[ __DDRx_MAX_ENTRYS__ ] =
	{
		[eDDRA] = &DDRA,
		[eDDRB] = &DDRB,
		[eDDRC] = &DDRC,
		[eDDRD] = &DDRD,
	};
	
	return pDDRxLookup[ eDDR ];
}

/*****************************************************************/


// end of file
