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
    eSHIFT_REG_2,
    eSHIFT_REG_3,

    __MAX_eSHIFT_ENTRYS__
};

typedef struct
{
  uint8_t* pRegStart; /*!<-- Erstes virtuelles Schieberegister <--*/
  uint8_t uiLeftRegs; /*!<-- Anzahl der verbleibenden Register <--*/
}sShiftRegInfo_t;
/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eShiftRegError_t ShiftRegToggleBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegSetBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegClearBit( uint8_t uiBitPos );

eShiftRegError_t ShiftRegSet( enum eShiftRegs eMember , uint8_t uiValue );

sShiftRegInfo_t ShiftRegGet( enum eShiftRegs eMember );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SHIFTREG_H__

