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


#define _FUNCTION_INPUT                 0x10
#define _FUNCTION_INPUT_PULLUP          0x20
#define _FUNCTION_OUTPUT                0x40

#define HIGH							0x80
#define LOW                             !HIGH					

#define IO_TAB_DEFAULT_MASK		        0x80
#define IO_TAB_PIN_FUNC_MASK	        0x70
#define IO_TAB_PIN_MASK			        0x07



#define IO_TAB_CNFG( PINx , DEFAULT_STATE , FUNCx )	( ( PINx ) | ( DEFAULT_STATE ) | ( FUNCx ) )


typedef volatile uint8_t* pRegister_t;

typedef struct
{
    pRegister_t		   pPort;
    volatile uint8_t      uiFuncBitPos; // High Byte = Pin Funktion , Low Byte = Pin Nummer
}sIO_t;

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

void IoTabInit( const sIO_t *sTab );

void IoTabSetLow( const sIO_t *sTab );

void IoTabSetHigh( const sIO_t *sTab );

void IoTabxHigh( const sIO_t *sTab , uint8_t uiMember );

void IoTabxLow( const sIO_t *sTab , uint8_t uiMember );

void IoTabxToggle( const sIO_t *sTab , uint8_t uiMember );

/*****************************************************************/


// end of file



#endif // __IOTAB_H__
