/*#################################################################################################
	Für		: Auslesen DS1621 / DS1631
	Version	: 1.0
	autor	: Martin Junghans	info@jtronics.de   www.jtronics.de
	License	: GNU General Public License 
	Hardware: ATmega128, 16MHz,
	Aufgabe	: Auslesen der Temperatursensoren DS1621/ DS1631 und kompatible Chips per I2C Bus
//################################################################################################*/
#ifndef DS1621_H
#define DS1621_H

#include <stdint.h>

/*########################################################################################## chip adress
		MSb 	Bit6 	Bit5 	Bit4 	Bit3 	Bit2 	Bit1 	LSb
		1		0		0		1		A2		A1		A0		R/W
				
//########################################################################################## Status Bit
		MSb 	Bit6 	Bit5 	Bit4 	Bit3 	Bit2 	Bit1 	LSb
		DONE 	THF 	TLF 	NVB 	X 		X 		POL 	1SHOT

DONE = 	Conversion Done bit. “1” = Conversion complete, “0” = Conversion in progress.

THF = 	Temperature High Flag. This bit will be set to “1” when the temperature is greater than or
		equal to the value of TH. It will remain “1” until reset by writing “0” into this location or removing power
		from the device. This feature provides a method of determining if the DS1621 has ever been subjected to
		temperatures above TH while power has been applied.

TLF = 	Temperature Low Flag. This bit will be set to “1” when the temperature is less than or equal
		to the value of TL. It will remain “1” until reset by writing “0” into this location or removing power from
		the device. This feature provides a method of determining if the DS1621 has ever been subjected to
		temperatures below TL while power has been applied.

NVB = 	Nonvolatile Memory Busy flag. “1” = Write to an E2 memory cell in progress, 
		“0” = nonvolatile memory is not busy. A copy to E2 may take up to 10 ms.

X = 	Reserved.

X = 	Reserved.

POL = 	Output Polarity Bit. “1” = active high, “0” = active low. This bit is nonvolatile.

1SHOT = One Shot Mode. If 1SHOT is “1”, the DS1621 will perform one temperature conversion upon
		receipt of the Start Convert T protocol. If 1SHOT is “0”, the DS1621 will continuously perform
		temperature conversions. This bit is nonvolatile.

//################################################################################################*/

#define CMD_START_CONVERT  	0b11101110	// --> start the temperature conversion.
#define CMD_READ_TEMP      	0b10101010 	// read the temperature from the ds1621 device. After executing this command, you can read out a two-byte-value.
#define CMD_ACCESS_CONFIG  	0b10101100	// read the configuration register. Returns a one-byte-value, so you can use ds1621_retreg().
#define CMD_READ_COUNTER   	0b10101000	// read the temperature counter. Returns a one-byte-value, so you can use ds1621_retreg().
#define CMD_READ_SLOPE     	0b10101001	// read the slope counter. Returns a one-byte-value, so you can use ds1621_retreg().

void ds1621_init(uint8_t addr); 					// Start the temperature conversion
int16_t ds1621_get_temp(uint8_t addr);				// Read the DS1621 --> return temperature
int16_t ds1621_get_temp_h(uint8_t addr);			// Read the DS1621 --> return temperature with high resolution.
void ds1621_sethigh(uint8_t addr, int16_t temp);	// Obere Thermostatgrenze setzen 27,5°C --> 275
void ds1621_setlow(uint8_t addr, int16_t temp);		// Untere Thermostatgrenze setzen  27,5°C --> 275

#endif
