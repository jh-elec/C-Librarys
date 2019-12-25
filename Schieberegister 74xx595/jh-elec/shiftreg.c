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

volatile sShiftRegPort_t* sShiftRegPORT = (sShiftRegPort_t*)&_SHIFT_REG_PORT;

/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
void ShiftRegInit( void )
{
	/*!<-- Daten Richtungs Register Adresse <--*/
	volatile sShiftRegPort_t* sShiftRegDDR = ( sShiftRegPORT - 1 );
	
	/*!<-- Pins als Ausgänge konfigurieren <--*/
	sShiftRegDDR->bData = 1;
	sShiftRegDDR->bOutputEnable = 1;
	sShiftRegDDR->bShiftClock = 1;
	sShiftRegDDR->bStoreClock = 1;
}


void ShiftRegUpdate( void )
{
	uint8_t* _pData = pShiftReg + __MAX_eSHIFT_ENTRYS__;

	sShiftRegPORT->bOutputEnable = 0;

	for ( uint8_t y = 0 ; y < __MAX_eSHIFT_ENTRYS__ ; y++ )
	{
		for ( uint8_t x = 0 ; x < 8 ; x++ )
		{
			sShiftRegPORT->bShiftClock = 0;
			if ( *_pData )
			{
				sShiftRegPORT->bData = 1;
			}
			else
			{
				sShiftRegPORT->bData = 0;
			}
			sShiftRegPORT->bShiftClock = 0;	
		}
		
		/*!<-- Daten übernehmen <--*/
		sShiftRegPORT->bStoreClock = 1;
		sShiftRegPORT->bStoreClock = 0;
		
		_pData--;
	}
}

eShiftRegError_t ShiftRegToggleBit( uint8_t uiBitPos )
{
    eShiftRegError_t eState = eSHIFT_REG_OK;

    uint8_t uiNumOfReg = uiBitPos >> 3;	/*!<-- >> 3 entspricht x/8 <--*/
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

    uint8_t uiNumOfReg = uiBitPos >> 3; /*!<-- >> 3 entspricht x/8 <--*/
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
    uint8_t uiNumOfReg = uiBitPos >> 3; /*!<-- >> 3 entspricht x/8 <--*/
    uint8_t uiBitOfReg = uiBitPos & 0x07;

    if ( uiNumOfReg > __MAX_eSHIFT_ENTRYS__ )
    {
        return eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[uiNumOfReg] &= ~( 1 << uiBitOfReg );

    return eSHIFT_REG_OK;
}

eShiftRegError_t ShiftRegSetByte( enum eShiftRegs eMember , uint8_t uiValue )
{
    if ( eMember > __MAX_eSHIFT_ENTRYS__ )
    {
        return eSHIFT_REG_OUT_OF_RANGE;
    }

    pShiftReg[eMember] = uiValue;

    return eSHIFT_REG_OK;
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


