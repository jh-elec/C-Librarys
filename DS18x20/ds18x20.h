/*************************************************************
*|
*|	\@file  	ds18x20.h
*|	\@brief 	Aktuell wurde nur der "DS18B20" gestestet
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	API DS18x20
*|
*|	\@date		21/12/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.211219
*|
*|**************************************************************/

#ifndef __DS18X20_H__
#define __DS18X20_H__


#include <stdio.h>
#include <avr/io.h>
#include "onewire.h"

/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/
typedef struct  
{
	uint8_t uiSlaves; /*!<-- Wie viele Slaves wurden nach "Ds18B20Init" gefunden! <--*/
}sDs18B20_t;

sDs18B20_t sDs18B20;

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eStatuscode_t Ds18B20Init( void );

int16_t Ds18B20GetTemp( eStatuscode_t *peStatus );

eStatuscode_t Ds18B20ReadSlaveTemp( enum eSlavesOnBus eSlave , int16_t *pTemp );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __DS18X20_H__


