
#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <avr/pgmspace.h>

#include "mcp2515.h"
#include "mcp2515_defs.h"


uint8_t Mcp2515Write( uint8_t *Frame , uint8_t Lenght )
{
	if( Lenght < 2 )
	{
		return 1; // Zu wenig Daten übergeben
	}
	
	MCP2515_CS_LOW;
	for( uint8_t i = 0 ; i < Lenght ; i++ ) 
	{
		// Daten senden via. SPI
	}
	MCP2515_CS_HIGH;
	
	return 0; // Alles inordnung
}

uint8_t	Mcp2515Read( uint8_t Adress , uint8_t *Buffer , uint8_t Lenght )
{
	/*	**Info**
	*	Adress: Register Adresse die gelesen werden soll
	*	Buffer: Hier werden die Daten gespeichert
	*	Lenght: Nur die Länge der Bytes die gelesen werden sollen
	*/
	
	if( Buffer == NULL || Lenght < 1 )
	{
		return 1; // Ungültige Übergabeparameter
	}
	
	uint8_t Frame[] = { SPI_READ , Adress };
	Mcp2515Write( Frame , sizeof(Frame) ); // Lese Modus + Register Adresse
	
	MCP2515_CS_LOW;
	for( uint8_t i = 0 ; i < Lenght ; i++ )
	{
		Frame[i] = 
	}
	MCP2515_CS_HIGH;
	
	return 0; // Alles inordnung
}

void Mcp2515BitModify(uint8_t Adress , uint8_t Mask , uint8_t Data )
{	
	MCP2515_CS_LOW;
	
	uint8_t Frame[] = { SPI_BIT_MODIFY , Adress , Mask , Data };
	
	Mcp2515Write(Frame,sizeof(Frame));
	
	MCP2515_CS_HIGH;
}

uint8_t Mcp2515ReadState(uint8_t Type)
{
	uint8_t Data;
	
	MCP2515_CS_LOW;
	
	Mcp2515Read(&Type,&Data,1);
	
	MCP2515_CS_HIGH;
	
	return Data;
}

uint8_t Mcp2515Init(uint8_t Bitrate)
{
	if( Bitrate >= 8 )
	{
		return 1;
	}
	
	MCP2515_INT_DDR	|= (1<<MCP2515_INT_BP);
	MCP2515_CS_DDR  |= (1<<MCP2515_CS_BP);
		
	// reset MCP2515 by software reset.
	// After this he is in configuration mode.
	Mcp2515Write(&SPI_RESET,1);
	
	// wait a little bit until the MCP2515 has restarted
	_delay_us(10);
	
	
	MCP2515_CS_LOW;
	
	uint8_t Frame[]={SPI_WRITE,CNF3,0,0,0,MCP2515_INTERRUPTS}; // 0 = Platzhalter für Timinig Werte
	
	for(uint8_t i = 0 ; i < 3 ; i++)
	{
		Frame[2+x] = Mcp2515SpeedCnfg[Bitrate][i];
	}
	
	Mcp2515Write(Frame,sizeof(Frame)); // Init schreiben
	
	MCP2515_CS_HIGH;

	
	// TXnRTS Bits als Inputs schalten
	Frame[0] = SPI_WRITE;
	Frame[1] = TXRTSCTRL;
	Frame[2] = 0;
	
	Mcp2515Write(Frame,3);
	
	
	#if defined(MCP2515_INT)
		SET_INPUT(MCP2515_INT);
		SET(MCP2515_INT);
	#endif
	
	#ifdef RXnBF_FUNKTION
		MCP2515_RX0BF_DDR &= ~(1<<MCP2515_RX0BF_BP);
		MCP2515_RX1BF_DDR &= ~(1<<MCP2515_RX1BF_BP);
		
		/* PullUps aktivieren */
		MCP2515_RX0BF_PORT |= (1<<MCP2515_RX0BF_BP);
		MCP2515_RX1BF_PORT |= (1<<MCP2515_RX1BF_BP);
		
		// Aktivieren der Pin-Funktionen fuer RX0BF und RX1BF
		// Frame[0] sollte hier noch "SPI_WRITE" beeinhalten
		Frame[1] = BFPCTRL;
		Frame[2] = (1<<B0BFE)|(1<<B1BFE)|(1<<B0BFM)|(1<<B1BFM);
		Mcp2515Write(Frame,3);
		
	#else
		#ifdef MCP2515_TRANSCEIVER_SLEEP
			// activate the pin RX1BF as GPIO which is connected 
			// to RS of MCP2551 and set it to 0
			// Frame[0] sollte hier noch "SPI_WRITE" beeinhalten
			Frame[1] = BFPCTRL;
			Frame[2] = 1<<B1BFE);
			Mcp2515Write(Frame,3);
		#else
			// Deaktivieren der Pins RXnBF Pins (High Impedance State)
			// Frame[0] sollte hier noch "SPI_WRITE" beeinhalten
			Frame[1] = BFPCTRL;
			Frame[2] = 0;
			Mcp2515Write(Frame,3);
		#endif
	#endif
		
	// Device zurueck in den normalen Modus versetzten
	// und aktivieren/deaktivieren des Clkout-Pins
	// Frame[0] sollte hier noch "SPI_WRITE" beeinhalten
	Frame[1] = CANCTRL;
	Frame[2] = CLKOUT_PRESCALER_;
	Mcp2515Write(Frame,3);
	
	
	uint8_t Result = 0;
	uint32_t Timeout = 10e6;
	while ( (Result & 0xE0) != 0 && Timeout-- > 0 ) 
	{
		// warten bis der neue Modus uebernommen wurde
		Mcp2515Read(CANSTAT , &Result , 1 );
	}
	
	if(Timeout == 0) 
	{
		return 2;
	}
		
	return 0;
}

uint8_t Mcp2515CheckNewMessage(void) 
{
	/* INT Pin auswerten */
	return (!(MCP2515_INT_PORT) & (1<<MCP2515_INT_BP));
}

uint8_t Mcp2515CheckFreeBuffer(void)
{
	uint8_t Status = Mcp2515ReadState(SPI_READ_STATUS);
	
	if ((Status & 0x54) == 0x54) 
	{
		// Alle Puffer sind in benutzung..
		return 0;
	}
	
	return 1;// Mindestens 1 Puffer ist frei..
}

uint8_t mcp2515_get_message(tCAN *Message)
{
	uint8_t Status = Mcp2515ReadState(SPI_RX_STATUS);
	uint8_t Address;
	uint8_t t;
	
	if( ((Status & 1<<6) == 1) ) 
	{
		// Nachricht in Puffer 0
		Address = SPI_READ_RX;
	}
	else if ( ((Status & 1<<7) == 1) ) 
	{
		// Nachricht in Puffer 1
		Address = (SPI_READ_RX | 0x04);
	}
	else 
	{
		// Error: Keine neue Nachricht vorhanden
		return 0;
	}

	MCP2515_CS_LOW;
	
	uint8_t ReceiveFrame[2] = "";
	
	Mcp2515Read( Address , ReceiveFrame , 2 );
	
	// Empfangenen Nachrichten "Idetifier" lesen
	Message->Id  = (uint16_t) Frame[0] << 3;
	Message->Id |=            Frame[1] >> 5;
	
	uint8_t Frame[] = { 0xFF , 0xFF };
	Mcp2515Write( Frame , 2 );
	
	
	// DLC lesen
	uint8_t Length = spi_putc(0xFF) & 0x0F;
	
	Message->Header.Length = Length;
	Message->Header.Rtr = (bit_is_set(Status, 3)) ? 1 : 0;
	
	// read data
	for ( t = 0 ; t < Length ; t++ ) 
	{
		Message->Data[t] = spi_putc(0xff);
	}
	MCP2515_CS_HIGH;
	
	// clear interrupt flag
	if (bit_is_set(Status, 6)) 
	{
		mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
	}
	else 
	{
		mcp2515_bit_modify(CANINTF, (1<<RX1IF), 0);
	}
	
	return (Status & 0x07) + 1;
}

uint8_t Mcp2515SendMessage(tCAN *Message)
{
	uint8_t Status = Mcp2515ReadState(SPI_READ_STATUS);
	
	/* Statusbyte:
	 *
	 * Bit	Function
	 *  2	TXB0CNTRL.TXREQ
	 *  4	TXB1CNTRL.TXREQ
	 *  6	TXB2CNTRL.TXREQ
	 */
	 
	uint8_t Address;
	uint8_t t;
	
	if(((Status & 1<<2) == 0)) 
	{
		Address = 0x00;
	}
	else if (((Status & 1<<4) == 0)) 
	{
		Address = 0x02;
	} 
	else if (((Status & 1<<6) == 0)) 
	{
		Address = 0x04;
	}
	else 
	{
		/* Alle Puffer sind in benutzung, es kann gerade keine 
		*  Nachricht gesendet werden 
		*/
		return 0;
	}
	
	MCP2515_CS_LOW;
	
	uint8_t Frame[16] = 
	{ 
		SPI_WRITE_TX | Address, 
		Message->id >> 3, 
		Message->id << 5, 
		0, 
		0 
	};
	
	uint8_t Length = Message->Header.Length & 0x0F;
	
	if (Message->Header.Rtr) 
	{
		// Ein RTR (Remote Transmission Request) Frame hat eine Länge von x aber enhält kein Nutzdatenfeld
		Frame[5] = ((1<<RTR) | Length);
	}
	else 
	{
		// Länge der Nachricht 
		Frame[6] = (Length);
		
		// Daten aus dem Frame
		for(t = 0 ; t < Length ; t++ ) 
		{
			Frame[7+t] = (Message->Data[t]);
		}
	}
	
	Mcp2515Write(Frame,sizeof(Frame));
	
	MCP2515_CS_HIGH;
	
	_delay_us(1);
	
	// send message
	MCP2515_CS_LOW;
	
	Address = (Address == 0) ? 1 : Address;
	
	Address |= SPI_RTS;
	
	Mcp2515Write(&Address , 1);
	
	MCP2515_CS_HIGH;
	
	return Address;
}
