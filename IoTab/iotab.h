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
#define __IOTAB_H__

#include <stdio.h>
#include <avr/io.h>


/*!<-- defines -- >*/
/*****************************************************************/

#define __PACK_IT__	                    __attribute__ ( ( packed ) )

#define IO_CALC_PORT_ADDR( ADDRx )		( __SFR_OFFSET + ADDRx ) // Es geht bei den I/O´s erst ab einer bestimmten Adresse los!

#define IOTAB_PORT( ADDRx )             (*((volatile uint8_t*) (uintptr_t)(ADDRx)))
#define IOTAB_DDR( ADDRx )              (*((volatile uint8_t*) (uintptr_t)(ADDRx-1)))
#define IOTAB_PIN( ADDRx )              (*((volatile uint8_t*) (uintptr_t)(ADDRx+1)))

 
enum ePinFunc
{
    _FUNCTION_INPUT,
	_FUNCTION_INPUT_PULLUP,
	_FUNCTION_INPUT_PULLDOWN,	
	
    _FUNCTION_OUTPUT,
	
	__MAX_FUNCTION_ENTRYS__
	
}__PACK_IT__;


typedef struct
{
    volatile uint8_t   *uiPort;
    const uint8_t      uiBitPos;
    const uint8_t      uiPinFunc :3;
}sIO_t;


enum ePorts  
{
	ePORTA = 0,
	ePORTB,
	ePORTC,
	ePORTD,
	__PORTS_MAX_ENTRYS__
}__PACK_IT__;

enum eDDRs
{
	eDDRA = 0,
	eDDRB,
	eDDRC,
	eDDRD,
	__DDRx_MAX_ENTRYS__
}__PACK_IT__;


enum eIoTabPos
{
	IOTAB_POS_LED1,
	IOTAB_POS_K1,
	IOTAB_POS_BEEPER,
}__PACK_IT__;

typedef volatile uint8_t* pRegister_t;

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/

sIO_t IoTable[]=
{
	[IOTAB_POS_LED1]   = { &PORTA , 2 , _FUNCTION_OUTPUT },	
	[IOTAB_POS_K1]     = { &PORTB , 3 , _FUNCTION_OUTPUT },
	[IOTAB_POS_BEEPER] = { &PORTC , 4 , _FUNCTION_OUTPUT },	
};

uint8_t uiTabSize = sizeof( IoTable ) / sizeof( *IoTable );

/*****************************************************************/


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



#endif // __IOTAB_H__
