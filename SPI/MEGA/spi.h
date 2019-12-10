/*************************************************************
*|
*|	\@file  	spi.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	SPI Schnittstellen Treiber (ATMEL AVR´s)
*|
*|	\@date		10/12/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.101219
*|
*|**************************************************************/

#ifndef __SPI_H__
#define __SPI_H__


#include <stdio.h>
#include <avr/io.h>


/*!<-- Defines <--*/
/*****************************************************************/
#define _SPI_GET_DDR( PORTx )		( * ( &PORTx - 1 ) )

//#define SPI_CS_ENABLE

#if defined ( __AVR_ATmega32__ )
	
	#define SPI_PORT		PORTB
	#define SPI_SCK_bp		PB7
	#define SPI_MISO_bp		PB6
	#define SPI_MOSI_bp		PB5
	
	/*
	*	Software Slave Select Signal
	*	Muss bei dem Master selbst organisiert werden.
	*	Der "SS" Hardware Pin wird nur bei dem Slave verwendet.
	*/
	#define SPI_SS_PORT		PORTC
	#define SPI_SS_bp		PC1

#elif defined (__AVR_ATmega168PA__ )

	#define SPI_PORT		PORTB
	#define SPI_SCK_bp		PB5
	#define SPI_MISO_bp		PB4
	#define SPI_MOSI_bp		PB3
	
	/*
	*	Software Slave Select Signal
	*	Muss bei dem Master selbst organisiert werden.
	*	Der "SS" Hardware Pin wird nur bei dem Slave verwendet.
	*/
	#define SPI_SS_PORT		PORTB
	#define SPI_SS_bp		PB2
#else
	#error "no definition for MCU"
#endif

enum eSpiPrescaler
{
	eSPI_PRESCALER_4,
	eSPI_PRESCALER_16,
	eSPI_PRESCALER_64,
	eSPI_PRESCALER_128,
	
	eSPI_DBL_SPEED_PRESCALER_2,
	eSPI_DBL_SPEED_PRESCALER_8,
	eSPI_DBL_SPEED_PRESCALER_32,
	eSPI_DBL_SPEED_PRESCALER_64,
	
	__MAX_eSPI_PRESCALER_ENTRYS__
};

enum eSpiDoubleSpeedState
{
	eSPI_DOUBLE_SPEED_OFF,
	eSPI_DOUBLE_SPEED_ON,
};

enum eClockCnfg
{
	eCLOCK_CNFG_1,
	eCLOCK_CNFG_2,
	eCLOCK_CNFG_3,
	eCLOCK_CNFG_4,
	
	__MAX_eCLOCK_ENTRYS__
};

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Interne Variablen <--*/
/*****************************************************************/
static const uint8_t pClockCnfg[__MAX_eCLOCK_ENTRYS__] =
{
	[eCLOCK_CNFG_1] = ~( ( 1<<CPOL ) | ( 1<<CPHA ) ),
	[eCLOCK_CNFG_2] = ( 1<<CPHA ),
	[eCLOCK_CNFG_3] = ( 1<<CPOL ),
	[eCLOCK_CNFG_4] = ( ( 1<<CPOL ) | ( 1<<CPHA ) ),
};

static const uint8_t pSpiPrescaler[__MAX_eSPI_PRESCALER_ENTRYS__] =
{
	[eSPI_PRESCALER_4]				= ~( ( 1<<SPR1 ) | ( 1<<SPR0 ) ),
	[eSPI_PRESCALER_16]				= ( 1<<SPR0 ),
	[eSPI_PRESCALER_64]				= ( 1<<SPR1 ),
	[eSPI_PRESCALER_128]			= ( ( 1<<SPR1 ) | ( 1<<SPR0 ) ),
	
	[eSPI_DBL_SPEED_PRESCALER_2]	= ( ( 1<<SPR1 ) | ( 1<<SPR0 ) ),
	[eSPI_DBL_SPEED_PRESCALER_8]	= ( 1<<SPR0 ),
	[eSPI_DBL_SPEED_PRESCALER_32]	= ( 1<<SPR1 ),
	[eSPI_DBL_SPEED_PRESCALER_64]	= ( ( 1<<SPR1 ) | ( 1<<SPR0 ) )
};
/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktions Prototypen <--*/
/*****************************************************************/
uint8_t SpiMasterInit( enum eSpiPrescaler eSpiPrescaler , enum eClockCnfg eClockCnfg , enum eSpiDoubleSpeedState eDblSpeed );
void	SpiMasterWrite( uint8_t data );
uint8_t SpiMasterRead( uint8_t data );
uint8_t SpiMasterWriteRead( uint8_t data );
void	SpiMasterSlaveSelect( void );
void	SpiMasterSlaveSelect( void );
/*****************************************************************/
/*!<-- Funktions Prototypen // Ende <--*/


// end of file



#endif // __SPI_H__


