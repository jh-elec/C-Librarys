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

#define IOTAB_DDR( DDRx )               ( DDRx  - 1 )
#define IOTAB_PIN( PINx )               ( PINx  + 1 )
#define IOTAB_PORT( PORTx )             ( PORTx + 0 ) 


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
	{ IO_CALC_PORT_ADDR(0x12) , 2 , _FUNCTION_OUTPUT },
	{ IO_CALC_PORT_ADDR(0x12) , 3 , _FUNCTION_OUTPUT },
};

static const uint8_t uiTabSize = sizeof( IoTable ) / sizeof( *IoTable );

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

extern inline void IoTabInit( sIO_t *sTab , uint8_t uiMembers )
{	
	uint8_t *pDDR , *pPORT , *pPIN  = NULL;	
	
	for ( uint8_t x = 0 ; x < uiMembers ; x++ )
	{
		pDDR  = (uint8_t*)((sTab[x].uiPort)-1);
		pPORT = (uint8_t*)((sTab[x].uiPort)+0);
		pPIN  = (uint8_t*)((sTab[x].uiPort)+1);
	
		switch ( sTab[x].eFunction )
		{
			case _FUNCTION_INPUT:
			{
				*pDDR &= ~( 1 << sTab[x].uiBitPos );
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				*pDDR &= ~( 1 << sTab[x].uiBitPos );
				*pPIN |= ( 1 << sTab[x].uiBitPos );
			}break;
			
			case _FUNCTION_INPUT_PULLDOWN:
			{
				// Noch nicht vorgesehen!
				// bzw. Gibt es bei den ATmega nicht
			}break;
			
			case _FUNCTION_OUTPUT:
			{					
				*pDDR  |= ( 1 << sTab[x].uiBitPos );
				*pPORT |= ( 1 << sTab[x].uiBitPos );
				
			}break;
			
			case __MAX_FUNCTION_ENTRYS__:{}				
		}
	}
}

/*****************************************************************/


// end of file



#endif // __IOTAB_H__
