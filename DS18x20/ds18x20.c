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
int16_t Ds18B20ReadTemp( eStatuscode_t *peStatus ) 
{	
	enum eOrder{ eORDER_LSB , eORDER_MSB , __MAX_eORDER_ENTRYS__ };

	int16_t siTemp;
	uint8_t uiBuffer[__MAX_eORDER_ENTRYS__];

	*peStatus = eSTATUS_OK;

	/*!<-- Temperatur Messung starten <--*/
	if ( OneWireSkipROM() != eSTATUS_ACK )
	{
		*peStatus = eSTATUS_NO_ACK;
	}
	OneWireWriteByte( eCOMMAND_CONVERT );
	
	/*!<-- Dem Sensor Zeit geben um die Tempereatur zu ermitteln.. <--*/
	_delay_ms(750);

	if ( OneWireSkipROM() != eSTATUS_ACK )
	{
		*peStatus = eSTATUS_NO_ACK;
	}
	
	OneWireWriteByte( eCOMMAND_READ_SCRATCHPAD );
	
	uiBuffer[eORDER_LSB] = OneWireReadByte();
	uiBuffer[eORDER_MSB] = OneWireReadByte();
	
	/*!<-- lesen beenden <--*/
	if ( OneWireReset() != eSTATUS_ACK )
	{
		//*peStatus = eSTATUS_NO_ACK;
	}

	siTemp = ((int16_t)uiBuffer[eORDER_MSB] << 8) | uiBuffer[eORDER_LSB];
	siTemp = ( siTemp * 640L ) >> 10;
	
	return siTemp;
}
/*****************************************************************/
/*!<-- Funktionen  // Ende <--*/


// end of file


