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

#define IO_CALC_PORT_ADDR( ADDRx )		( __SFR_OFFSET + ADDRx ) // Es geht bei den I/O´s erst ab einer bestimmten Adresse los!

#define IOTAB_PORT( ADDRx )  (*((volatile uint8_t*) (uintptr_t)(ADDRx)))
#define IOTAB_DDR( ADDRx )   (*((volatile uint8_t*) (uintptr_t)(ADDRx-1)))
#define IOTAB_PIN( ADDRx )   (*((volatile uint8_t*) (uintptr_t)(ADDRx+1)))

 
typedef enum
{
    _FUNCTION_INPUT,
	_FUNCTION_INPUT_PULLUP,
	_FUNCTION_INPUT_PULLDOWN,	
	
    _FUNCTION_OUTPUT,
	
	__MAX_FUNCTION_ENTRYS__
	
}eFunction_t;


typedef struct
{
    uint8_t            uiPort;
    const uint8_t      uiBitPos;
    const eFunction_t  eFunction;
}sIO_t;


typedef enum ePorts
{
	ePORTA = 0,
	ePORTB,
	ePORTC,
	ePORTD,
	__PORTS_MAX_ENTRYS__
}ePorts_t;

typedef enum eDataDirectionRegisters
{
	eDDRA = 0,
	eDDRB,
	eDDRC,
	eDDRD,
	__DDRx_MAX_ENTRYS__
}eDDRx_t;


typedef volatile uint8_t* pRegister_t;

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/

/*!
*   Der Offset der "SpecialFunctionRegister (SFR)" wird mit dem 
*   Macro "IO_CALC_PORT_ADDR()" vom Compiler selbst berechnet.
*->*/
sIO_t IoTable[] =
{
	{ IO_CALC_PORT_ADDR(0x12) , 2 , _FUNCTION_OUTPUT },
	{ IO_CALC_PORT_ADDR(0x12) , 3 , _FUNCTION_OUTPUT },
	{ IO_CALC_PORT_ADDR(0x12) , 4 , _FUNCTION_OUTPUT },		
	{ IO_CALC_PORT_ADDR(0x12) , 5 , _FUNCTION_OUTPUT },		
};

static const uint8_t uiTabSize = sizeof( IoTable ) / sizeof( *IoTable );

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

extern inline void IoTabInit( sIO_t *sTab , uint8_t uiMembers )
{	
	for ( uint8_t x = 0 ; x < uiMembers ; x++ )
	{
		switch ( sTab[x].eFunction )
		{
			case _FUNCTION_INPUT:
			{
				IOTAB_DDR( sTab[x].uiPort ) &= ~( 1 << sTab[x].uiBitPos );
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				IOTAB_DDR( sTab[x].uiPort ) &= ~( 1 << sTab[x].uiBitPos );
				IOTAB_PIN( sTab[x].uiPort ) |= ( 1 << sTab[x].uiBitPos );
			}break;
			
			case _FUNCTION_INPUT_PULLDOWN:
			{
				// Noch nicht vorgesehen!
				// bzw. Gibt es bei den ATmega nicht
			}break;
			
			case _FUNCTION_OUTPUT:
			{					
				IOTAB_DDR(sTab[x].uiPort) |= ( 1 << sTab[x].uiBitPos );
				IOTAB_PORT(sTab[x].uiPort) |= ( 1 << sTab[x].uiBitPos );
			}break;
			
			case __MAX_FUNCTION_ENTRYS__:{}				
		}
	}
}


extern inline pRegister_t getPort( ePorts_t ePort )
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

extern inline pRegister_t getDDRx( eDDRx_t eDDRx )
{
	if ( eDDRx > __DDRx_MAX_ENTRYS__ )
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
	
	return pDDRxLookup[ eDDRx ];
}

/*****************************************************************/


// end of file



#endif // __IOTAB_H__
