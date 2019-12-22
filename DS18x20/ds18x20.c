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

static void Ds18B20StartConvert( void )
{	
	OneWireWriteByte( eCOMMAND_ONEWIRE_CONVERT );
}



void Ds18B20ReadScratchpad( uint8_t *pBuffer ) 
{
	OneWireWriteByte( eCOMMAND_ONEWIRE_READ_SCRATCHPAD );
	for ( uint8_t x = 0 ; x < 9 ; x++ ) 
	{
		pBuffer[x] = OneWireReadByte();
	}
}

void Ds18B20WriteScratchpad( uint8_t uiTh , uint8_t uiTl )
{
	OneWireWriteByte( eCOMMAND_ONEWIRE_WRITE_SCRATCHPAD );
	OneWireWriteByte( uiTh );
	OneWireWriteByte( uiTl );
}

eStatuscode_t Ds18B20ReadTemp( int16_t *pTemp ) 
{	
    int16_t temp;
    uint8_t uiScratchpad[9] = "";
    
    Ds18B20ReadScratchpad( uiScratchpad );
	
    if ( OneWireCRC( uiScratchpad , 9 ) ) 
	{
	    return eSTATUS_ONEWIRE_CRC_ERROR;
    }

    temp = ((int16_t)uiScratchpad[1] << 8) | uiScratchpad[0];
    
    switch( ( uiScratchpad[4] >> 5 ) & 3 ) 
	{
	    case 0: temp &= ~7; break;    // 9 Bit
	    case 1: temp &= ~3; break;    // 10 Bit
	    case 2: temp &= ~1; break;    // 11 Bit
    }
    // calculate temperature with 0.1 C resolution using fixed point arithmetic
    // t(0.1C)  = t(1/16C) * 10/16
    *pTemp = (temp * 10) >> 4;
	
    return eSTATUS_ONEWIRE_OK;
}

int16_t Ds18B20GetTemp( eStatuscode_t *peStatus )
{
	int16_t siTemp;
	
	Ds18B20StartConvert();
	_delay_ms(750); /*!<-- Zeit geben zum ermitteln der Temperatur <--*/
	
	return Ds18B20ReadTemp( &siTemp );	
}

eStatuscode_t Ds18B20ReadSlaveTemp( enum eSlavesOnBus eSlave , int16_t *pTemp )
{
	eStatuscode_t eStatus = eSTATUS_ONEWIRE_OK;

	eStatus = OneWireMatchROM( OneWireMultiRom[ eSlave ] );
	Ds18B20StartConvert();
	_delay_ms( 750 );
	eStatus = OneWireMatchROM( OneWireMultiRom[ eSlave ] );
	eStatus = Ds18B20ReadTemp( pTemp );
	
	return eStatus;
}

/*****************************************************************/
/*!<-- Funktionen  // Ende <--*/


// end of file


