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

#include "../Includes//shiftreg.h"

/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Interne Variablen <--*/
/*****************************************************************/
static uint8_t pShiftReg[__MAX_eSHIFT_ENTRYS__] = "";

volatile sShiftRegPort_t* sShiftPort = (sShiftRegPort_t*)&_SHIFT_REG_PORT;

/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eShiftRegError_t ShiftRegSend( uint8_t *pData , uint8_t uiLength )
{
	eShiftRegError_t sError = eSHIFT_REG_OK;
	
	uint8_t* _pData = pData + __MAX_eSHIFT_ENTRYS__;

	sShiftPort->bOutputEnable = 0;

	for ( uint8_t y = 0 ; y < uiLength ; y++ )
	{
		for ( uint8_t x = 0 ; x < 8 ; x++ )
		{
			sShiftPort->bShiftClock = 0;
			if ( *_pData )
			{
				sShiftPort->bData = 1;
			}
			else
			{
				sShiftPort->bData = 0;
			}
			sShiftPort->bShiftClock = 0;	
		}
		
		/*!<-- Daten übernehmen <--*/
		sShiftPort->bStoreClock = 1;
		sShiftPort->bStoreClock = 0;
		
		pData--;
	}
	
	return sError;
}

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


