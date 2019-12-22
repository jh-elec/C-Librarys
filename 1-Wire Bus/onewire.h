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
	 eSTATUS_ONEWIRE_OK					= 0,        /*!<-- Kein Fehler aufgetreten (: <--*/
	 eSTATUS_ONEWIRE_NO_PRESENCE		= 1,        /*!<-- Slave antwortet nicht :'( <--*/
	 eSTATUS_ONEWIRE_CRC_ERROR			= 2,        /*!<-- Es ist ein CRC Fehler aufgetreten <--*/
	 eSTATUS_ONEWIRE_SCAN_ERROR			= 3,        /*!<-- Fehler während der ROM suche aufgetreten <--*/
	 eSTATUS_ONEWIRE_LAST_CODE			= 4,        /*!<-- Der letzte ROM Kode wurde wieder gescannt, nur eine Info ( kein Fehler ) <--*/
	 eSTATUS_ONEWIRE_GND_SHORT			= 5,        /*!<-- Der Bus ist wahrscheinlich gegen Masse kurzgeschlossen! <--*/
};

enum eCommands
{
	eCOMMAND_ONEWIRE_SEARCH_ROM			= 0xF0,
	eCOMMAND_ONEWIRE_READ_ROM			= 0x33,
	eCOMMAND_ONEWIRE_SKIP_ROM			= 0xCC,	
	eCOMMAND_ONEWIRE_MATCH_ROM			= 0x55,
	eCOMMAND_ONEWIRE_CONVERT			= 0x44,
	eCOMMAND_ONEWIRE_WRITE_SCRATCHPAD	= 0x4E,
	eCOMMAND_ONEWIRE_READ_SCRATCHPAD	= 0xBE,
	eCOMMAND_ONEWIRE_ALARM_SEARCH		= 0xEC,
	
	eCOMMAND_ONEWIRE_UNKNOWN			= 0xFF,
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
	
	uint8_t uiBusMembers; /*!<-- Am Bus angeschlossene Geräte <--*/
}sOneWire_t;

sOneWire_t sOneWire;

typedef uint8_t OneWireROM[8];

/*!<-- ROM Speicher für einen Slave am "1-Wire" Bus <--*/
OneWireROM OneWireRom;

/*!<-- ROM Speicher für mehrere Slaves am "1-Wire" Bus <--*/
OneWireROM OneWireMultiRom[__MAX_eSLAVE_ENTRYS__];

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
void OneWireInit( void );

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

eStatuscode_t OneWireScanBus( OneWireROM pROM );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __ONEWIRE_H__


