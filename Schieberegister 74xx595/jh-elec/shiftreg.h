/*************************************************************
*|
*|	\@file  	shiftreg.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Puffer fuer ein "virtuelles" Schieberegisters ( 8-Bit Breite! )
*|
*|	\@date		06/12/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.061219
*|
*|**************************************************************/

#ifndef __SHIFTREG_H__
#define __SHIFTREG_H__


#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

/*!<-- Defines <--*/
/*****************************************************************/
typedef enum
{
    eSHIFT_REG_OK,
    eSHIFT_REG_OUT_OF_RANGE,
}eShiftRegError_t;

enum eShiftRegs
{
    eSHIFT_REG_1,

    __MAX_eSHIFT_ENTRYS__
};

typedef struct
{
  uint8_t* pRegStart; /*!<-- Erstes virtuelles Schieberegister <--*/
  uint8_t uiLeftRegs; /*!<-- Anzahl der verbleibenden Register <--*/
}sShiftRegInfo_t;

/*!<-- Hier die Belegung des Schieberegisters Hinterlegen <--*/
/*!<-- ACHTUNG! Das ganze ist Compiler abhängig. Sollte jedoch beim AVR klappen <--*/

#define _SHIFT_REG_PORT		PORTB

// [0][1][2][3][4][5][6][7] <- Port Bits
typedef struct  
{
	uint8_t bOutputEnable	:1; 	
	uint8_t bStoreClock		:1;	
	uint8_t bShiftClock		:1;	
	uint8_t bData			:1; 	
	uint8_t b4				:1;
	uint8_t	b5				:1;
	uint8_t b6				:1; 
	uint8_t b7				:1; 
}sShiftRegPort_t;

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/
volatile sShiftRegPort_t* sShiftPort;
/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eShiftRegError_t ShiftRegSend( uint8_t *pData , uint8_t uiLength );

eShiftRegError_t ShiftRegToggleBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegSetBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegClearBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegSet( enum eShiftRegs eMember , uint8_t uiValue );

sShiftRegInfo_t ShiftRegGet( enum eShiftRegs eMember );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SHIFTREG_H__

