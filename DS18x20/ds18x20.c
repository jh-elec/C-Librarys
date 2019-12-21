/*************************************************************
*|
*|	\@file  	ds18x20.c
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

#include "../Headers/ds18x20.h"

/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktionen <--*/
/*****************************************************************/

static eStatuscode_t Ds18B20StartTemp( void )
{	
	eStatuscode_t eStatus = eSTATUS_OK;
	
	if ( OneWireReset() != eSTATUS_ACK )
	{
		return eSTATUS_NO_ACK;
	}
	
	/*!<-- Temperatur Messung starten <--*/
	if ( OneWireSkipROM() != eSTATUS_ACK )
	{
		eStatus = eSTATUS_NO_ACK;
	}
	OneWireWriteByte( eCOMMAND_CONVERT );
	
	_delay_ms(750);
	
	return eStatus;
}

static int16_t Ds18B20ReadTemp( eStatuscode_t *peStatus ) 
{	
	enum eOrder{ eORDER_LSB , eORDER_MSB , __MAX_eORDER_ENTRYS__ };

	int16_t siTemp;
	uint8_t uiBuffer[__MAX_eORDER_ENTRYS__];

	*peStatus |= eSTATUS_OK;
	
	OneWireWriteByte( eCOMMAND_READ_SCRATCHPAD );
	
	uiBuffer[eORDER_LSB] = OneWireReadByte();
	uiBuffer[eORDER_MSB] = OneWireReadByte();
	
	/*!<-- lesen beenden <--*/
	if ( OneWireReset() != eSTATUS_ACK )
	{
		*peStatus |= eSTATUS_NO_ACK;
	}

	siTemp = ((int16_t)uiBuffer[eORDER_MSB] << 8) | uiBuffer[eORDER_LSB];
	siTemp = ( siTemp * 640L ) >> 10;
	
	return siTemp;
}


int16_t Ds18B20GetTemp( eStatuscode_t *peStatus )
{
	*peStatus |= Ds18B20StartTemp();
	_delay_ms(750); /*!<-- Zeit geben zum ermitteln der Temperatur <--*/
	
	return Ds18B20ReadTemp( peStatus );	
}

int16_t Ds18B20ReadSlaveTemp( eStatuscode_t *peStatus , enum eSlavesOnBus eSlave )
{
	int16_t siTemp;

	OneWireSearchInit( OneWireRom );

	for ( uint8_t x = 0 ; x < __MAX_eSLAVE_ENTRYS__ ; x++ )
	{
		uint8_t uiStatus = OneWireSearchROM( OneWireRom );
		
		if (  uiStatus == eSTATUS_NEW_ROM_CODE_FOUND || uiStatus == eSTATUS_NO_NEW_ROM_CODE_FOUND )
		{
			OneWireSaveROM( OneWireRom , x );
		}	
	}
	
	OneWireMatchROM( OneWireMultiRom[ eSlave ] );
	*peStatus |= Ds18B20StartTemp();
	OneWireMatchROM( OneWireMultiRom[ eSlave ] );
	siTemp = Ds18B20ReadTemp( peStatus );
	
	return siTemp;
}

/*****************************************************************/
/*!<-- Funktionen  // Ende <--*/


// end of file


