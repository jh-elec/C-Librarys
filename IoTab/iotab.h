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

enum __PACK_IT__ ePinFunc
{
    _FUNCTION_INPUT              = 0x01,
	_FUNCTION_INPUT_PULLUP       = 0x02,
	_FUNCTION_INPUT_PULLDOWN     = 0x04,	
    _FUNCTION_OUTPUT             = 0x08,
};

/*!<-- High Byte( 0xF0 ) = Funktion des Pins , Low Byte( 0x0F ) = Pin Position -- >*/
#define IO_TAB_PINFUNC( PINx , FUNCx )	( ( FUNCx << 4 ) | ( PINx & 0x0F ) )


typedef volatile uint8_t* pRegister_t;

typedef struct
{
    pRegister_t		   uiPort;
    const uint8_t      uiFuncBitPos; // High Byte = Pin Funktion , Low Byte = Pin Nummer
}sIO_t;

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

void IoTabInit( const sIO_t *sTab );

void IoTabSetLow( const sIO_t *sTab );

void IoTabSetHigh( const sIO_t *sTab );

void IoTabxHigh( const sIO_t *sTab , uint8_t uiMember );

void IoTabxLow( const sIO_t *sTab , uint8_t uiMember );

/*****************************************************************/


// end of file



#endif // __IOTAB_H__
