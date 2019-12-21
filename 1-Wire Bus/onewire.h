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

/*!<-- Wie viele Slaves sind am "1-Wire" Bus angeschlossen? <--*/
enum eSlavesOnBus
{
	eSLAVE_1,
	eSLAVE_2,
	
	__MAX_eSLAVE_ENTRYS__	
};

enum eStatuscode
{
	eSTATUS_OK						= 0x01,
	eSTATUS_ACK						= 0x02,
	eSTATUS_NO_ACK					= 0x04,	
	eSTATUS_INVALID_CONFIG			= 0x08,	
	eSTATUS_BUS_SHORT_TO_GND		= 0x10,
	eSTATUS_NEW_ROM_CODE_FOUND		= 0x20,
	eSTATUS_NO_NEW_ROM_CODE_FOUND	= 0x40,
	eSTATUS_CRC_ERROR				= 0x80,
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
	uint8_t		b0			:1;
	uint8_t		b1			:1;
	uint8_t		b2			:1;
	uint8_t		b3			:1;
	uint8_t		bDQ			:1;
	uint8_t		b5			:1;
	uint8_t		b6			:1;
	uint8_t		bStateLED	:1;	
}sOneWireMapping_t;


typedef struct  
{
	volatile sOneWireMapping_t	*psDDR;
	volatile sOneWireMapping_t	*psPORT;
	volatile sOneWireMapping_t	*psPIN;
}sOneWire_t;

sOneWire_t sOneWire;

typedef uint8_t OneWireROM[8];

/*!<-- ROM Speicher für einen Slave am "1-Wire" Bus <--*/
OneWireROM OneWireRom;

/*!<-- ROM Speicher für mehrere Slaves am "1-Wire" Bus <--*/
OneWireROM OneWireMultiRom[__MAX_eSLAVE_ENTRYS__];

uint8_t test[2][8];

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

void OneWireSearchInit( OneWireROM pROM );

uint8_t OneWireSearchROM( OneWireROM pROM );

uint8_t OneWireSearchAlarm( OneWireROM pROM );

uint8_t OneWireSearch( OneWireROM pROM , enum eCommands eCommand );

eStatuscode_t OneWireMatchROM( const OneWireROM pROM );

eStatuscode_t OneWireSkipROM( void );

eStatuscode_t OneWireReadROM( OneWireROM pROM );

uint8_t OneWireCRC( const uint8_t *pDataIn , uint8_t uiCnt );

uint8_t OneWireSaveROM( OneWireROM ROM , enum eSlavesOnBus eSlave );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __ONEWIRE_H__


