/*************************************************************
*|
*|	\@file  	onewire.h
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

#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__


#include <stdio.h>
#include <avr/io.h>


/*!<-- Defines <--*/
/*****************************************************************/
#ifndef F_CPU
#define F_CPU  16e6       
#endif

#define _ONEWIRE_PORT						PORTB
#define _ONEWIRE_DATA_BP					PB1

#define _ONEWIRE_DELAY_US_( USx )          ( unsigned int )( F_CPU / 12.0 / 1e6 / 2 * USx )

enum eStatuscode
{
	eSTATUS_OK			= 1,
	eSTATUS_ERROR		= 2,
	eSTATUS_NO_PORT		= 3,	
	eSTATUS_ACK			= 4,
	eSTATUS_NO_ACK		= 5,
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

enum eSlaveAddr
{
	eSLAVE_ADDR_ONLY_ONE_SLAVE = 255	
};

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Globale Variablen <--*/
/*****************************************************************/

typedef uint8_t bool;
typedef enum eStatuscode eStatuscode_t;
typedef enum eSlaveAddr	 eSlaveAddr_t;
typedef uint8_t OneWireRomTab_t;

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
	sOneWireMapping_t	*sMapping;
	OneWireRomTab_t		pRomCode[8];
}sOneWire_t;


/*!<-- Hier werden die ROM Identifizierer des jeweiligen Slaves eingetragen,
		sollten mehrere an einem BUS angeschlossen sein <--*/
static const OneWireRomTab_t OneWireRomTab[][8] =
{
	{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // Slave 1
};

/*****************************************************************/
/*!<-- Globale Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
void OneWireDelay( uint16_t uiDelay );

eStatuscode_t OneWireInit( void );

eStatuscode_t OneWireReset( void );

void OneWireWriteBit( bool bValue );

uint8_t OneWireReadBit( void );

void OneWireWrite( uint8_t uiData );

uint8_t OneWireRead( void );

eStatuscode_t OneWireReadROM( void );

void OneWireSelectSlave( eSlaveAddr_t eSlaveAddr );

void OneWireGetROM( uint8_t *pDataOut );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __ONEWIRE_H__


