/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> MCP23S17
* Version           -> 1.0.0.0319
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 19.03.2018 13:50:03
* Description       -> Treiber f�r MCP23S17
*
*	�bertragungsprotokoll.:
*
*	Lesen ->		Byte[0] = Slave Adresse + lesen Bit
*					Byte[1]	= Adresse des zu lesenden Registers
*				
*					Die gelesenen Daten werden in den �bergabepuffer zur�ck geschrieben
*					An erster Stelle (Byte[0]) findet man den Anfang der empfangenen Daten.
*					Der Puffer mit gen�gend Speicher muss selbst sichergestellt werden.
*
*					F�r das lesen wird "mcp23s17ReadBytes ( uint8_t *buff , uint8_t leng )" benutzt!
*
*
*	Schreiben ->	Byte[0] = Slave Adresse + schreiben Bit
*					Byte[1] = Adresse des beschreibenden Registers
*					Byte[2..n] = Hier folgen die Daten die gesendet werden sollen
*
*					F�r das schreiben wird "uint8_t mcp23s17WriteBytes( uint8_t *buff , uint8_t leng )" benutzt!
*
*					Die beiden Funktionen liefern jeweils die Anzahl der �bertragenen Daten zur�ck.
*					Die Anzahl der zu lesenden / schreibenden Bytes kann mit "sizeof(buffer)" �bergeben werden.
*/

#include <avr/io.h>
#include "mcp23s17.h"
#include "xmega_spi.h"



void mcp23s17Init			( void )								
{
	MCP23S17_SS_PORT.DIRSET = 1<<MCP23S17_SS_bp;
	
	mcp23s17Cnfg( 1<<INTPOL | 1<<ODR | 1<<HAEN);
	
	/*
	*	Alle I/O�s auf Ausg�nge Programmieren
	*/
	mcp23s17CnfgOut( 0x00 , 0x00 );
	
	/*
	*	Alle Ausg�nge auf default setzen
	*/
	mcp23s17SetOut ( 0x00 , 0x00 );
}

uint8_t mcp23s17WriteBytes	( uint8_t *buff , uint8_t leng )		
{
	uint8_t i = 0;

	/*
	*	When using SPI Mode 1,1 the CS pin needs to be toggled once before the first communication after
	*	power-up.
	*/
	mcp23s17Deselect();
	mcp23s17Select();
	
	for ( i = 0 ; i < leng ; i++ )
	{
		spiMasterWrite( buff[i] );
	}
	
	mcp23s17Deselect();

	return i;
}

uint8_t mcp23s17ReadBytes	( uint8_t *buff , uint8_t leng )		
{
	uint8_t i;

	/*
	*	When using SPI Mode 1,1 the CS pin needs to be toggled once before the first communication after
	*	power-up.
	*/
	mcp23s17Deselect();
	mcp23s17Select();
	
	spiMasterWrite( *( &buff[0] ) ); // Slave Adresse + lesen / schreiben
	spiMasterWrite( *( &buff[1] ) ); // Register Adresse	

	if ( leng == 1 )
	{
		buff[0] = spiMasterRead();
		return 0;
	}

	for ( i = 0 ; i < leng ; i++ )
	{
		buff[i] = spiMasterRead();
	}
	
	mcp23s17Deselect();

	return i;
}

uint8_t mcp23s17CnfgOut		( uint8_t gpioA ,  uint8_t gpioB )		
{
	uint8_t buff[] =
	{
		MCP23S17_OPCODE + SPI_WRITE,
		BANK0_IODIRA,
		gpioA,
		gpioB
	};
	
	/*
	*	Anzahl der geschrieben Bytes zur�ckgeben
	*/
	return ( mcp23s17WriteBytes( buff , sizeof( buff ) ) );
}

uint8_t mcp23s17SetOut		( uint8_t gpioA , uint8_t gpioB )		
{
	uint8_t buff[] =
	{
		MCP23S17_OPCODE + SPI_WRITE,
		BANK0_GPIOA,
		gpioA,
 		gpioB
	};
	
	/*
	*	Anzahl der geschrieben Bytes zur�ckgeben
	*/
	return ( mcp23s17WriteBytes( buff , sizeof( buff ) ) );	
}

uint8_t mcp23s17Cnfg		( enum mcp23s17_cnfg_bits cnfgByte )	
{
	uint8_t buff[] =
	{
		MCP23S17_OPCODE + SPI_WRITE,
		BANK0_IOCON0A,
		cnfgByte
	};	
		
	/*
	*	Anzahl der geschrieben Bytes zur�ckgeben
	*/	
	return mcp23s17WriteBytes( buff , sizeof( buff ) );
}

void mcp23s17Select( void )		
{
	MCP23S17_SS_PORT.OUTCLR = 1<<MCP23S17_SS_bp;
}

void mcp23s17Deselect( void )	
{
	MCP23S17_SS_PORT.OUTSET = 1<<MCP23S17_SS_bp;
}