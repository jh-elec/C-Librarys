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


#define IOTAB_GET_PORT(ADDRx)			( * ( &ADDRx     ) )
#define IOTAB_GET_PIN(PORTx)			( * ( &PORTx + 1 ) )
#define IOTAB_GET_DDR(PORTx)            ( * ( &PORTx - 1 ) )

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

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/

/*!
*   Der Offset der "SpecialFunctionRegister (SFR)" wird mit dem 
*   Macro "IO_CALC_PORT_ADDR()" vom Compiler selbst berechnet.
*->*/
sIO_t IoTable[] =
{
	{ IO_CALC_PORT_ADDR(0x12) , 0 , _FUNCTION_INPUT },
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
				IOTAB_GET_DDR( sTab[x].uiPort ) &= ~( 1 << sTab[x].uiBitPos ); 
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				IOTAB_GET_DDR( sTab[x].uiPort ) &= ~( 1 << sTab[x].uiBitPos ); 
				IOTAB_GET_PIN( sTab[x].uiPort ) |= ( 1 << sTab[x].uiBitPos ); 
			}break;
			
			case _FUNCTION_INPUT_PULLDOWN:
			{
				// Noch nicht vorgesehen!
				// bzw. Gibt es bei den ATmega nicht
			}break;
			
			case _FUNCTION_OUTPUT:
			{
				IOTAB_GET_DDR ( sTab[x].uiPort ) &= ~( 1 << sTab[x].uiBitPos ); 
				IOTAB_GET_PORT( sTab[x].uiPort ) |= ( 1 << sTab[x].uiBitPos ); 
			}break;
			
			case __MAX_FUNCTION_ENTRYS__:{}				
		}
	}
}

/*****************************************************************/


// end of file



#endif // __IOTAB_H__
