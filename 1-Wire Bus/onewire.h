/*************************************************************
*|
*|	\@file  	onewire.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	API für das "1-Draht BUS System"
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

#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__


#include <stdio.h>
#include <string.h>
#include <avr/io.h>


/*!<-- Defines <--*/
/*****************************************************************/
#ifndef F_CPU
#define F_CPU  16e6       
#endif

#include <util/delay.h>

#define _ONEWIRE_GET_DDR( PORTx )			( * ( &PORTx - 1 ) )
#define _ONEWIRE_GET_PIN( PORTx )			( * ( &PORTx - 2 ) )

#define _ONEWIRE_PORT						PORTB
#define _ONEWIRE_DATA_BP					PB1

enum eStatuscode
{
	eSTATUS_OK					= 1,
	eSTATUS_ERROR				= 2,
	eSTATUS_INVALID_CONFIG		= 3,	
	eSTATUS_ACK					= 4,
	eSTATUS_NO_ACK				= 5,
	eSTATUS_BUS_SHORT_TO_GND	= 6,
	eSTATUS_NEW_ROM_CODE_FOUND	= 7,
	eSTATUS_CRC_ERROR			= 8,
};

enum eCommands
{
	eCOMMAND_SEARCH_ROM			= 0xF0,
	eCOMMAND_READ_ROM			= 0x33,
	eCOMMAND_SKIP_ROM			= 0xCC,	
	eCOMMAND_MATCH_ROM			= 0x55,
	eCOMMAND_CONVERT			= 0x44,
	eCOMMAND_WRITE_SCRATCHPAD	= 0x4E,
	eCOMMAND_READ_SCRATCHPAD	= 0xBE,
	eCOMMAND_ALARM_SEARCH		= 0xEC,
};


/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

typedef enum eStatuscode eStatuscode_t;

typedef struct
{
	uint8_t		bData;
	uint8_t		b1;
	uint8_t		b2;
	uint8_t		b3;
	uint8_t		b4;
	uint8_t		b5;
	uint8_t		b6;
	uint8_t		b7;	
}sOneWireMapping_t;


typedef struct  
{
	sOneWireMapping_t	*psDDR;
	sOneWireMapping_t	*psPORT;
	sOneWireMapping_t	*psPIN;
}sOneWire_t;

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
eStatuscode_t OneWireInit( void );

eStatuscode_t OneWireReset( void );

void OneWireWriteBit( uint8_t bBit );

uint8_t OneWireReadBit( void );

uint8_t OneWireReadByte( void );

void OneWireWriteByte( uint8_t uiData );

void OneWireSearchInit( uint8_t uiBuffer[8] );

uint8_t OneWireSearchROM( uint8_t uiBuffer[8] );

uint8_t OneWireSearchAlarm( uint8_t uiBuffer[8] );

uint8_t OneWireSearch( uint8_t uiBuffer[8] , enum eCommands eCommand );

eStatuscode_t OneWireMatchROM( const uint8_t uiBufferROM[8] );

eStatuscode_t OneWireSkipROM( void );

eStatuscode_t OneWireReadROM( uint8_t uiBufferROM[8] );

uint8_t OneWireCRC( const uint8_t *pDataIn , uint8_t uiCnt );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __ONEWIRE_H__


