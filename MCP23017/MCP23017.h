
/************************************************************************/
/* Needs the I2C-Lib form Peter Fleury                                  */
/************************************************************************/

/************************************************************************/
/* Function prototypes                                                  */
/************************************************************************/
 void mcp23017_reg_write (int mcp23017_address, int regnumber, int regvalue);
 void mcp23017_reg_read (int mcp23017_address, int regnumber);
/************************************************************************/ 

/* ALL IN BYTEMODE (all regs alone) (=> IOCON.BANK = 1)
 * 
 * configurating over own mind over just a function called
 * void mcp23017_reg_write (int mcp23017_address, int regnumber, int regvalue)
 *
 * Reading over the function
 * void mcp23017_reg_read (int mcp23017_address, int regnumber)
 * Returnal is register value.
 *
 * regnumber is defined as e.g. #define OLATB 0x15;
 */

//I2C slave address |L|H|L|L|A2|A1|A0|R/W|
//I/O data bus |P7|P6|P5|P4|P3|P2|P1|P0

/************************************************************************/
/*																		*/                                                                      
/* Address			| Address		|									*/
/* IOCON.BANK = 1	| IOCON.BANK = 0| Access to:						*/ 
/* 00h				| 00h			| IODIRA							*/
/* 10h				| 01h			| IODIRB							*/
/* 01h 				| 02h 			| IPOLA								*/
/* 11h 				| 03h 			| IPOLB								*/
/* 02h 				| 04h 			| GPINTENA							*/
/* 12h 				| 05h 			| GPINTENB							*/
/* 03h 				| 06h 			| DEFVALA							*/
/* 13h 				| 07h 			| DEFVALB							*/
/* 04h 				| 08h 			| INTCONA							*/
/* 14h 				| 09h 			| INTCONB							*/
/* 05h 				| 0Ah 			| IOCON								*/
/* 15h 				| 0Bh 			| IOCON								*/
/* 06h 				| 0Ch 			| GPPUA								*/
/* 16h 				| 0Dh 			| GPPUB								*/
/* 07h 				| 0Eh 			| INTFA								*/
/* 17h 				| 0Fh 			| INTFB								*/
/* 08h 				| 10h 			| INTCAPA							*/
/* 18h 				| 11h 			| INTCAPB							*/
/* 09h 				| 12h 			| GPIOA								*/
/* 19h 				| 13h 			| GPIOB								*/
/* 0Ah 				| 14h 			| OLATA								*/
/* 1Ah 				| 15h 			| OLATB								*/
/*																		*/
/************************************************************************/

//_________________________________________________________________________________________________


/*
Text in comments like this are copyright by microchip.
*/

/*
The datadirectionregister IODIRn defines the datadirection
on the port n.
When a bit is set, the corresponding pin becomes an
input. When a bit is clear, the corresponding pin
becomes an output.
*/
#define IODIRA		0x00; 
#define IODIRB		0x10; 

/*
The input-polarity-register allows the user to 
configure the polarity on the corresponding GPIO
port bits.
If a bit is set, the corresponding GPIO register bit will
reflect the inverted value on the pin.
*/
#define IPOLA		0x01; 
#define IPOLB		0x11;

/*
The GPINTEN register controls the interrupt-onchange
feature for each pin.
If a bit is set, the corresponding pin is enabled for
interrupt-on-change. The DEFVAL and INTCON
registers must also be configured if any pins are
enabled for interrupt-on-change.
*/ 
#define GPINTENA	0x02; 
#define GPINTENB	0x12; 

/*
The default comparison value is configured in the
DEFVAL register. If enabled (via GPINTEN and
INTCON) to compare against the DEFVAL register, an
opposite value on the associated pin will cause an
interrupt to occur.
*/
#define DEFVALA		0x03; 
#define DEFVALB		0x13;

/*
The INTCON register controls how the associated pin
value is compared for the interrupt-on-change feature.
If a bit is set, the corresponding I/O pin is compared
against the associated bit in the DEFVAL register. If a
bit value is clear, the corresponding I/O pin is compared
against the previous value.
*/ 
#define INTCONA		0x04; 
#define INTCONB		0x14;

/*
The IOCON register contains several bits for
configuring the device:

The BANK bit changes how the registers are mapped.
• If BANK = 1, the registers associated with each
port are segregated. Registers associated with
PORTA are mapped from address 00h - 0Ah and
registers associated with PORTB are mapped
from 10h - 1Ah.
• If BANK = 0, the A/B registers are paired. For
example, IODIRA is mapped to address 00h and
IODIRB is mapped to the next address (address
01h). The mapping for all registers is from 00h -
15h.
It is important to take care when changing the BANK bit
as the address mapping changes after the byte is
clocked into the device. The address pointer may point
to an invalid location after the bit is modified.
For example, if the device is configured to
automatically increment its internal Address Pointer,
the following scenario would occur:
• BANK = 0
• Write 80h to address 0Ah (IOCON) to set the
BANK bit
• Once the write completes, the internal address
now points to 0Bh which is an invalid address
when the BANK bit is set.
For this reason, it is advised to only perform byte writes
to this register when changing the BANK bit.

The MIRROR bit controls how the INTA and INTB pins
function with respect to each other.
• When MIRROR = 1, the INTn pins are functionally
OR’ed so that an interrupt on either port will cause
both pins to activate.
• When MIRROR = 0, the INT pins are separated.
Interrupt conditions on a port will cause its
respective INT pin to activate.

The Sequential Operation (SEQOP) controls the
incrementing function of the Address Pointer. If the
address pointer is disabled, the Address Pointer does
not automatically increment after each byte is clocked
during a serial transfer. This feature is useful when it is
desired to continuously poll (read) or modify (write) a
register.

The Slew Rate (DISSLW) bit controls the slew rate
function on the SDA pin. If enabled, the SDA slew rate
will be controlled when driving from a high to low.
The Hardware Address Enable (HAEN) bit enables/
disables hardware addressing on the MCP23S17 only.

The address pins (A2, A1 and A0) must be externally
biased, regardless of the HAEN bit value.
If enabled (HAEN = 1), the device’s hardware address
matches the address pins.
If disabled (HAEN = 0), the device’s hardware address
is A2 = A1 = A0 = 0.

The Open-Drain (ODR) control bit enables/disables the
INT pin for open-drain configuration. Erasing this bit
overrides the INTPOL bit.

The Interrupt Polarity (INTPOL) sets the polarity of the
INT pin. This bit is functional only when the ODR bit is
cleared, configuring the INT pin as active push-pull.
*/ 
#define IOCON		0x05; 
#define IOCON		0x15;

/*
The GPPU register controls the pull-up resistors for the
port pins. If a bit is set and the corresponding pin is
configured as an input, the corresponding port pin is
internally pulled up with a 100k resistor.
*/
#define GPPUA		0x06; 
#define GPPUB		0x16; 

/*
The INTF register reflects the interrupt condition on the
port pins of any pin that is enabled for interrupts via the
GPINTEN register. A ‘set’ bit indicates that the
associated pin caused the interrupt.
This register is ‘read-only’. Writes to this register will be
ignored.
*/
#define INTFA		0x07; 
#define INTFB		0x17; 

/*
The INTCAP register captures the GPIO port value at
the time the interrupt occurred. The register is ‘read
only’ and is updated only when an interrupt occurs. The
register will remain unchanged until the interrupt is
cleared via a read of INTCAP or GPIO.
*/
#define INTCAPA		0x08; 
#define INTCAPB		0x18; 

/*
The GPIO register reflects the value on the port.
Reading from this register reads the port. Writing to this
register modifies the Output Latch (OLAT) register.
*/
#define GPIOA		0x09; 	
#define GPIOB		0x19; 

/*
The OLAT register provides access to the output
latches. A read from this register results in a read of the
OLAT and not the port itself. A write to this register
modifies the output latches that modifies the pins
configured as outputs.
*/
#define OLATA		0x0A; 
#define OLATB		0x1A; 