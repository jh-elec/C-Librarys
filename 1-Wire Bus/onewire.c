/*************************************************************
*|
*|	\@file  	onewire.c
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	API für das "1-Draht Bus System"
*|
*|	\@date		16/12/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.161219
*|
*|**************************************************************/

#include "../Headers/onewire.h"


/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Interne Variablen <--*/
/*****************************************************************/
static sOneWire_t sOneWire;
/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktionen <--*/
/*****************************************************************/

void OneWireDelay( uint16_t uiDelay )
{
	while ( --uiDelay > 0 ){
	};
}

eStatuscode_t OneWireInit( void )
{
	if ( &_ONEWIRE_PORT == NULL )
	{
		return eSTATUS_NO_PORT;
	}
	
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		sOneWire.pRomCode[x] = 0;
	}
	
	/*!<-- Port mappen <--*/
	sOneWire.sMapping = (sOneWireMapping_t*)&_ONEWIRE_PORT;
	
	return eSTATUS_OK;
}

eStatuscode_t OneWireReset( void )
{
	eStatuscode_t eStatus = eSTATUS_ACK;
	
	sOneWire.sMapping->bData = 0;
	OneWireDelay( _ONEWIRE_DELAY_US_(496) );          
	sOneWire.sMapping->bData = 1;                 
	OneWireDelay( _ONEWIRE_DELAY_US_(65) );   
	
	if ( sOneWire.sMapping->bData ) /*!<-- Status einlesen <--*/
	{
		eStatus = eSTATUS_NO_ACK;
	}
	         
	OneWireDelay( _ONEWIRE_DELAY_US_(424) );  
	        
	return eStatus;             
}

void OneWireWriteBit( bool bValue )
{
	sOneWire.sMapping->bData = 0;
	if ( bValue )
	{
		sOneWire.sMapping->bData = 1;
	}
	OneWireDelay( _ONEWIRE_DELAY_US_(105) );
	sOneWire.sMapping->bData = 1;
}

uint8_t OneWireReadBit( void )
{
	bool bRead = 0;

	sOneWire.sMapping->bData = 0;
	sOneWire.sMapping->bData = 1;
	OneWireDelay( _ONEWIRE_DELAY_US_(15) );
	bRead = sOneWire.sMapping->bData;
	OneWireDelay( _ONEWIRE_DELAY_US_(105) );

	return bRead;              
}

void OneWireWrite( uint8_t uiData )
{
	uint8_t uiBitPos = 0x01;
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		if ( uiData & uiBitPos )
		{
			OneWireWriteBit( 1 );
		}
		else
		{
			OneWireWriteBit( 0 );
		}
		uiBitPos <<= 1;
	}
}

uint8_t OneWireRead( void )
{
	uint8_t uiData = 0x00;
	uint8_t uiBitPos = 0x01;
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		if ( OneWireReadBit() )
		{
			uiData |= uiBitPos;
		}
		uiBitPos <<= 1;
	}
	
	return uiData;
}

eStatuscode_t OneWireReadROM( void )
{
	eStatuscode_t eStatus = eSTATUS_OK;

	if( OneWireReset() )
	{
		eStatus = eSTATUS_NO_ACK;
		return eStatus;
	}

	OneWireWrite( eCOMMAND_READ_ROM );

	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		sOneWire.pRomCode[x] = OneWireRead();
	}
	
	return eStatus;
}

void OneWireSelectSlave( eSlaveAddr_t eSlaveAddr )
{
	switch ( eSlaveAddr )
	{
		case eSLAVE_ADDR_ONLY_ONE_SLAVE:
		OneWireWrite( eCOMMAND_SKIP_ROM );
		break;
		
		default:
		OneWireWrite( eCOMMAND_MATCH_ROM );
		for ( uint8_t x = 0 ; x < 8 ; x++ )
		{
			OneWireWrite( OneWireRomTab[eSlaveAddr][7-x] );
		}
	}
}

void OneWireGetROM( uint8_t *pDataOut )
{
	pDataOut = sOneWire.pRomCode;
}

/*****************************************************************/
/*!<-- Funktionen // Ende <--*/


// end of file




