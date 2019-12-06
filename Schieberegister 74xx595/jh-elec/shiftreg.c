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

#include "shiftreg.h"

/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Interne Variablen <--*/
/*****************************************************************/
static uint8_t pShiftReg[__MAX_eSHIFT_ENTRYS__] = "";
/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eShiftRegError_t ShiftRegToggleBit( uint8_t uiBitPos )
{
    eShiftRegError_t eState = eSHIFT_REG_OK;

    uint8_t uiNumOfReg = uiBitPos >> 3;
    uint8_t uiBitOfReg = uiBitPos & 0x07;

    if ( uiNumOfReg > __MAX_eSHIFT_ENTRYS__ )
    {
        eState = eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[uiNumOfReg] ^= ( 1 << uiBitOfReg );

    return eState;
}

eShiftRegError_t ShiftRegSetBit( uint8_t uiBitPos )
{
    eShiftRegError_t eState = eSHIFT_REG_OK;

    uint8_t uiNumOfReg = uiBitPos >> 3;
    uint8_t uiBitOfReg = uiBitPos & 0x07;

    if ( uiNumOfReg > __MAX_eSHIFT_ENTRYS__ )
    {
        eState = eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[uiNumOfReg] |= ( 1 << uiBitOfReg );

    return eState;
}

eShiftRegError_t ShiftRegClearBit( uint8_t uiBitPos )
{
    eShiftRegError_t eState = eSHIFT_REG_OK;

    uint8_t uiNumOfReg = uiBitPos >> 3;
    uint8_t uiBitOfReg = uiBitPos & 0x07;

    if ( uiNumOfReg > __MAX_eSHIFT_ENTRYS__ )
    {
        eState = eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[uiNumOfReg] &= ~( 1 << uiBitOfReg );

    return eState;
}

eShiftRegError_t ShiftRegSet( enum eShiftRegs eMember , uint8_t uiValue )
{
    eShiftRegError_t eState = eSHIFT_REG_OK;

    if ( eMember > __MAX_eSHIFT_ENTRYS__ )
    {
        eState = eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[eMember] = uiValue;

    return eState;
}

sShiftRegInfo_t ShiftRegGet( enum eShiftRegs eMember )
{
    sShiftRegInfo_t sInfo;

    if ( ( eMember > __MAX_eSHIFT_ENTRYS__ ) || ( ( __MAX_eSHIFT_ENTRYS__ - eMember ) < 0 ) )
    {
        sInfo.pRegStart = NULL;
        sInfo.uiLeftRegs = 0;
        return sInfo;
    }

    sInfo.uiLeftRegs = __MAX_eSHIFT_ENTRYS__ - eMember;
    sInfo.pRegStart = &pShiftReg[eMember];

    return sInfo;
}
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file


