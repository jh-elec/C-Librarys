
#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <avr/pgmspace.h>

#include "mcp2515.h"
#include "mcp2515_defs.h"


uint8_t Mcp2515Write( uint8_t *Frame , uint8_t Lenght )
{
	/*	**Struktur**
	*	Frame[0]	= SPI_WRITE Befehl	// ..
	*	Frame[1] 	= Register Adresse	// Diese beiden Befehle müssen gesendet werden!
	*	Frame[2..n] = Daten 			// Hier kommen die eigentlichen Daten..
	*/
	
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

typedef struct
{
	uint8_t PHSEG21;
	BTLMODE
	PHSEG11
	BRP2
}

uint8_t Mcp2515Init(uint8_t Speed)
{
	MCP2515_INT_DDR	|= (1<<MCP2515_INT_BP);
	MCP2515_CS_DDR  |= (1<<MCP2515_CS_BP);
		
	// reset MCP2515 by software reset.
	// After this he is in configuration mode.
	Mcp2515Write(&SPI_RESET,1);
	
	// wait a little bit until the MCP2515 has restarted
	_delay_us(10);
	
	
	MCP2515_CS_LOW;
	
	/*	Config
	*	Bitrate: 125kbps @ 16 MHz
	*/
	uint8_t Frame[] = 
	{ 
		SPI_WRITE, 
		CNFG3, 
		(1<<PHSEG21),
		(1<<BTLMODE)|(1<<PHSEG11),
		(1<<BRP2)|(1<<BRP1)|(1<<BRP0),
		Speed, // ???
		(1<<RX1IE)|(1<<RX0IE) // Interrupts aktivieren
	};
	
	Mcp2515Write(Frame , sizeof(Frame));
	
	MCP2515_CS_HIGH;

	
	// test if we could read back the value => is the chip accessible?
	if (mcp2515_read_register(CNF1) != speed) 
	{
		SET(LED2_HIGH);

		return false;
	}
	
	// deaktivate the RXnBF Pins (High Impedance State)
	mcp2515_write_register(BFPCTRL, 0);
	
	// set TXnRTS as inputs
	mcp2515_write_register(TXRTSCTRL, 0);
	
	// turn off filters => receive any message
	mcp2515_write_register(RXB0CTRL, (1<<RXM1)|(1<<RXM0));
	mcp2515_write_register(RXB1CTRL, (1<<RXM1)|(1<<RXM0));
	
	// reset device to normal mode
	mcp2515_write_register(CANCTRL, 0);
//	SET(LED2_HIGH);
	return true;
}

// ----------------------------------------------------------------------------
// check if there are any new messages waiting

uint8_t mcp2515_check_message(void) {
	return (!IS_SET(MCP2515_INT));
}

// ----------------------------------------------------------------------------
// check if there is a free buffer to send messages

uint8_t mcp2515_check_free_buffer(void)
{
	uint8_t status = mcp2515_read_status(SPI_READ_STATUS);
	
	if ((status & 0x54) == 0x54) {
		// all buffers used
		return false;
	}
	
	return true;
}

// ----------------------------------------------------------------------------
uint8_t mcp2515_get_message(tCAN *message)
{
	// read status
	uint8_t status = mcp2515_read_status(SPI_RX_STATUS);
	uint8_t addr;
	uint8_t t;
	if (bit_is_set(status,6)) {
		// message in buffer 0
		addr = SPI_READ_RX;
	}
	else if (bit_is_set(status,7)) {
		// message in buffer 1
		addr = SPI_READ_RX | 0x04;
	}
	else {
		// Error: no message available
		return 0;
	}

	RESET(MCP2515_CS);
	spi_putc(addr);
	
	// read id
	message->id  = (uint16_t) spi_putc(0xff) << 3;
	message->id |=            spi_putc(0xff) >> 5;
	
	spi_putc(0xff);
	spi_putc(0xff);
	
	// read DLC
	uint8_t length = spi_putc(0xff) & 0x0f;
	
	message->header.length = length;
	message->header.rtr = (bit_is_set(status, 3)) ? 1 : 0;
	
	// read data
	for (t=0;t<length;t++) {
		message->data[t] = spi_putc(0xff);
	}
	SET(MCP2515_CS);
	
	// clear interrupt flag
	if (bit_is_set(status, 6)) {
		mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
	}
	else {
		mcp2515_bit_modify(CANINTF, (1<<RX1IF), 0);
	}
	
	return (status & 0x07) + 1;
}

// ----------------------------------------------------------------------------
uint8_t mcp2515_send_message(tCAN *message)
{
	uint8_t status = mcp2515_read_status(SPI_READ_STATUS);
	
	/* Statusbyte:
	 *
	 * Bit	Function
	 *  2	TXB0CNTRL.TXREQ
	 *  4	TXB1CNTRL.TXREQ
	 *  6	TXB2CNTRL.TXREQ
	 */
	uint8_t address;
	uint8_t t;
//	SET(LED2_HIGH);
	if (bit_is_clear(status, 2)) {
		address = 0x00;
	}
	else if (bit_is_clear(status, 4)) {
		address = 0x02;
	} 
	else if (bit_is_clear(status, 6)) {
		address = 0x04;
	}
	else {
		// all buffer used => could not send message
		return 0;
	}
	
	RESET(MCP2515_CS);
	spi_putc(SPI_WRITE_TX | address);
	
	spi_putc(message->id >> 3);
    spi_putc(message->id << 5);
	
	spi_putc(0);
	spi_putc(0);
	
	uint8_t length = message->header.length & 0x0f;
	
	if (message->header.rtr) {
		// a rtr-frame has a length, but contains no data
		spi_putc((1<<RTR) | length);
	}
	else {
		// set message length
		spi_putc(length);
		
		// data
		for (t=0;t<length;t++) {
			spi_putc(message->data[t]);
		}
	}
	SET(MCP2515_CS);
	
	_delay_us(1);
	
	// send message
	RESET(MCP2515_CS);
	address = (address == 0) ? 1 : address;
	spi_putc(SPI_RTS | address);
	SET(MCP2515_CS);
	
	return address;
}
