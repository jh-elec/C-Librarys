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

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
int16_t Ds18B20ReadTemp( eStatuscode_t *peStatus );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __DS18X20_H__


